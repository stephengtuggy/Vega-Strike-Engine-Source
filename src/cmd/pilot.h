#include "config.h"
#include "gnuhash.h"
#include <vector>
class Animation;
class Unit;

class Pilot
{
    unsigned char gender; // which sound should play
    float         reaction_time;
    float         rank;
    int           faction; // duplicate data...any way round this??
  public:
    explicit Pilot(int faction);
    virtual ~Pilot()
    {
    }
    void       SetComm(std::shared_ptr<Unit> comm_unit); // so we can specialize base sort of people
    Animation *getCommFace(std::shared_ptr<Unit> parent, float moon, unsigned char &gender);
    float      getReactionTime()
    {
        return reaction_time;
    }
    unsigned char getGender()
    {
        return gender;
    }
    float getRank()
    {
        return rank;
    }                                                // man it's rank in here
    typedef vsUMap<const void *, float> relationmap; // non dereferencable Unit to float
    relationmap                         effective_relationship;
    std::vector<Animation *> *          comm_face;
    float                               getAnger(const std::shared_ptr<Unit> parent, const std::shared_ptr<Unit> un) const;
    std::vector<Animation *> *          getCommFaces(unsigned char &sex)
    {
        sex = gender;
        return comm_face;
    }
    float GetEffectiveRelationship(const std::shared_ptr<Unit> parent, const std::shared_ptr<Unit> target) const;
    float adjustSpecificRelationship(std::shared_ptr<Unit> parent, void *aggressor, float value, int guessedFaction /*pass in neutral otherwise*/);
    void  DoHit(std::shared_ptr<Unit> parent, void *aggressor, int guessedFaction /*pass in neutral otherwise*/);
};
