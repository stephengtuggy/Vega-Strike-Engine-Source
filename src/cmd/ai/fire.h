#ifndef _CMD_TARGET_AI_H_
#define _CMD_TARGET_AI_H_
#include "comm_ai.h"
#include "event_xml.h"
// all unified AI's should inherit from FireAt, so they can choose targets together.
bool  RequestClearence(class std::shared_ptr<Unit> parent, class std::shared_ptr<Unit> targ, unsigned char sex);
std::shared_ptr<Unit> getAtmospheric(std::shared_ptr<Unit> targ);
namespace Orders
{
class FireAt : public CommunicatingAI
{
  protected:
    bool         ShouldFire(std::shared_ptr<Unit> targ, bool &missilelock);
    float        missileprobability;
    float        lastmissiletime;
    float        delay;
    float        agg;
    float        distance;
    float        lastchangedtarg;
    bool         had_target;
    void         FireWeapons(bool shouldfire, bool lockmissile);
    virtual void ChooseTargets(int num, bool force = false); // chooses n targets and puts the best to attack in unit's target container
    bool         isJumpablePlanet(std::shared_ptr<Unit> );
    void         ReInit(float agglevel);
    virtual void SignalChosenTarget();

  public:
    // Other new Order functions that can be called from Python.
    virtual void ChooseTarget()
    {
        ChooseTargets(1, true);
    }
    void         PossiblySwitchTarget(bool istargetjumpableplanet);
    virtual bool PursueTarget(std::shared_ptr<Unit> , bool leader);
    void         AddReplaceLastOrder(bool replace);
    void         ExecuteLastScriptFor(float time);
    void         FaceTarget(bool end);
    void         FaceTargetITTS(bool end);
    void         MatchLinearVelocity(bool terminate, Vector vec, bool afterburn, bool local);
    void         MatchAngularVelocity(bool terminate, Vector vec, bool local);
    void         ChangeHeading(QVector vec);
    void         ChangeLocalDirection(Vector vec);
    void         MoveTo(QVector, bool afterburn);
    void         MatchVelocity(bool terminate, Vector vec, Vector angvel, bool afterburn, bool local);
    void         Cloak(bool enable, float seconds);
    void         FormUp(QVector pos);
    void         FormUpToOwner(QVector pos);
    void         FaceDirection(float distToMatchFacing, bool finish);
    void         XMLScript(std::string script);
    void         LastPythonScript();

    virtual void SetParent(std::shared_ptr<Unit> parent)
    {
        CommunicatingAI::SetParent(parent);
    }
    std::shared_ptr<Unit> GetParent()
    {
        return CommunicatingAI::GetParent();
    }
    FireAt(float aggressivitylevel); // weapon prefs?
    FireAt();
    virtual void        Execute();
    virtual std::string Pickle()
    {
        return std::string();
    } // these are to serialize this AI
    virtual void UnPickle(std::string)
    {
    }
    virtual ~FireAt();
};
} // namespace Orders
#endif
