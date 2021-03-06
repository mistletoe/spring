/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef __BUILDER_CAI_H__
#define __BUILDER_CAI_H__

#include <map>
#include <string>

#include "MobileCAI.h"
#include "Sim/Units/UnitSet.h"
#include "Sim/Units/BuildInfo.h"

class CUnit;
class CFeature;
class CSolidObject;
class CWorldObject;
struct Command;
struct UnitDef;

class CBuilderCAI : public CMobileCAI
{
public:
	CR_DECLARE(CBuilderCAI);
	CBuilderCAI(CUnit* owner);
	CBuilderCAI();
	~CBuilderCAI();
	void PostLoad();

	int GetDefaultCmd(const CUnit* unit, const CFeature* feature);
	void SlowUpdate();

	void DrawCommands();
	void GiveCommandReal(const Command& c, bool fromSynced = true);
	void DrawQuedBuildingSquares();

	/**
	 * @param noResCheck no resources check
	 * @param recUnits reclaims units and features
	 * @param recNonRez reclaims non resurrectable only
	 * @param recEnemy reclaims enemy units
	 * @param recEnemyOnly reclaims enemy units only
	 * @param recSpecial reclaims also non autoreclaimable, metal first
	 */
	bool FindReclaimTargetAndReclaim(const float3& pos, float radius,
			unsigned char options, bool noResCheck, bool recUnits,
			bool recNonRez, bool recEnemy, bool recEnemyOnly, bool recSpecial);
	/**
	 * @param freshOnly reclaims only corpses that have rez progress or all the metal left
	 */
	bool FindResurrectableFeatureAndResurrect(const float3& pos, float radius, unsigned char options, bool freshOnly);
	void FinishCommand();
	/**
	 * @param builtOnly skips units that are under construction
	 */
	bool FindRepairTargetAndRepair(const float3& pos, float radius, unsigned char options, bool attackEnemy, bool builtOnly);
	/**
	 * @param healthyOnly only capture units with capture progress or 100% health remaining
	 */
	bool FindCaptureTargetAndCapture(const float3& pos, float radius, unsigned char options, bool healthyOnly);

	void ExecutePatrol(Command& c);
	void ExecuteFight(Command& c);
	void ExecuteGuard(Command& c);
	void ExecuteStop(Command& c);
	virtual void ExecuteRepair(Command& c);
	virtual void ExecuteCapture(Command& c);
	virtual void ExecuteReclaim(Command& c);
	virtual void ExecuteResurrect(Command& c);
	virtual void ExecuteRestore(Command& c);

	bool ReclaimObject(CSolidObject* o);
	bool ResurrectObject(CFeature* feature);

	std::map<int, std::string> buildOptions;
	bool building;
	BuildInfo build;

	int cachedRadiusId;
	float cachedRadius;
	float GetUnitDefRadius(const UnitDef* unitdef, int cmdId);

	int buildRetries;

	int lastPC1; ///< helps avoid infinite loops
	int lastPC2;
	int lastPC3;

	bool range3D;

	inline float f3Dist(const float3& a, const float3& b) const {
		return range3D ? a.distance(b) : a.distance2D(b);
	}
	inline float f3SqDist(const float3& a, const float3& b) const {
		return range3D ? a.SqDistance(b) : a.SqDistance2D(b);
	}
	inline float f3Len(const float3& a) const {
		return range3D ? a.Length() : a.Length2D();
	}
	inline float f3SqLen(const float3& a) const {
		return range3D ? a.SqLength() : a.SqLength2D();
	}

public:
	static CUnitSet reclaimers;
	static CUnitSet featureReclaimers;
	static CUnitSet resurrecters;

private:

	void CancelRestrictedUnit(const std::string& buildOption);
	bool ObjInBuildRange(const CWorldObject* obj) const;
	bool OutOfImmobileRange(const Command& cmd) const;
	void ReclaimFeature(CFeature* f);

	/// fix for patrolling cons repairing/resurrecting stuff that's being reclaimed
	static void AddUnitToReclaimers(CUnit*);
	static void RemoveUnitFromReclaimers(CUnit*);

	/// fix for cons wandering away from their target circle
	static void AddUnitToFeatureReclaimers(CUnit*);
	static void RemoveUnitFromFeatureReclaimers(CUnit*);

	/// fix for patrolling cons reclaiming stuff that is being resurrected
	static void AddUnitToResurrecters(CUnit*);
	static void RemoveUnitFromResurrecters(CUnit*);
public:
	static bool IsUnitBeingReclaimed(CUnit* unit, CUnit* friendUnit = NULL);
	static bool IsFeatureBeingReclaimed(int featureId, CUnit* friendUnit = NULL);
	static bool IsFeatureBeingResurrected(int featureId, CUnit* friendUnit = NULL);

private:
};

#endif // __BUILDER_CAI_H__
