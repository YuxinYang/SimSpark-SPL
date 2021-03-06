/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef SOCCERRULEASPECT_H
#define SOCCERRULEASPECT_H

#include <soccercontrolaspect/soccercontrolaspect.h>
#include <soccertypes.h>
#include <ballstateaspect/ballstateaspect.h>
#include <gamestateaspect/gamestateaspect.h>

class AgentState;

namespace salt
{
    class AABB2;
}

namespace oxygen
{
    class RigidBody;
    class AgentAspect;
    class Transform;
}

class SoccerRuleAspect : public SoccerControlAspect
{
public:
    typedef std::list<boost::shared_ptr<AgentState> > TAgentStateList;

    enum EFoulType
    {
        FT_Crowding,
        FT_Touching,
        FT_IllegalDefence,
        FT_IllegalAttack,
        FT_Incapable,
        FT_KickOff,
        FT_Charging
    };

    struct Foul
    {
        Foul(unsigned _index, EFoulType _type, boost::shared_ptr<AgentState> _agent)
            : index(_index),
              type(_type),
              agent(_agent)
        {}
        int index;
        EFoulType type;
        boost::shared_ptr<AgentState> agent;
        bool operator<(Foul const& other) const { return index < other.index; }

        int time;
    };

public:
    SoccerRuleAspect();
    virtual ~SoccerRuleAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** Drop ball at its current position and move all players away
        by the free kick radius.
    */
    void DropBall();
    /** Drop ball at a given position and move all players away from that
        position by the free kick radius.
        \param pos position where the ball should be dropped-
    */
    void DropBall(salt::Vector3f pos);

    /** Calculates the out of the field reposition pos for a given agent with unum and team idx
        Agents are repositioned outside of the field near the mid field line on the opposite yy side
        regarding the ball position
    */
    salt::Vector3f RepositionOutsidePos(salt::Vector3f initPos, int unum, TTeamIndex idx);

    /** Calculates the inside field reposition pos for a given agent with unum and team idx
        Agents are repositioned at distance from the ball, that is, at: plpos + (plpos-ballpos).normalize()*dist
    */
    //salt::Vector3f RepositionInsidePos(salt::Vector3f initPos, int unum, TTeamIndex idx, float distance);

    /** New rules for repositioning players that commit fouls
    */
    void ClearPlayersAutomatic(TTeamIndex idx);

    /** Calculates distance arrays needed for repositioning players
    */
    void CalculateDistanceArrays(TTeamIndex idx);

    /** Calculates ordering on a distance vector */
    void SimpleOrder(float dArr[][3], int oArr[][3], TTeamIndex idx);

    /** Agent state concerining standing, laying down on the ground are processed
    */
    void ProcessAgentState(salt::Vector3f pos, int unum, TTeamIndex idx);

    /** Reset the foul time counter for all players and also other counters
    */
    void ResetFoulCounter(TTeamIndex idx);

    /** Reset the foul time counter for a given player
    */
    void ResetFoulCounterPlayer(int unum, TTeamIndex idx);

    /**Analyse Fouls from players and increase foul counter of offending players
    */
    void AnalyseFouls(TTeamIndex idx);

    /** Check whether too many agents are touching
     */
    void AnalyseTouchGroups(TTeamIndex idx);

    /** Reset the touch groups
    */
    void ResetTouchGroups(TTeamIndex idx);

    /** Automatic Referee that clears players that violate the rules
    */
    void AutomaticSimpleReferee(TPlayMode playMode);

    /** broadcast a said message to all players
        \param message said message-
        \param pos positon of the player-
        \param num uniform number-
        \param idx team index-
    */
    void Broadcast(const std::string& message, const salt::Vector3f& pos,
                   int number, TTeamIndex idx);

    /** Move all the players from a team which are too close to pos away
        from pos, with exception of the specified agent.
        \param pos the center of the area to be checked
        \param radius the radius of the area to be checked
        \param min_dist the minimum distance players will be moved away from pos
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field. If they would leave the playing field, they are moved
        towards the X axis (the line from the left goal to the right goal).
    */
    void ClearPlayersWithException(const salt::Vector3f& pos, float radius, float min_dist,
                      TTeamIndex idx, boost::shared_ptr<AgentState> agentState);

    /**
     * get the size of field, i.e. length and width
     *
     *
     * @return the length and width
     */
    salt::Vector2f GetFieldSize() const;

    /** Get the foul history
    */
    std::vector<Foul> GetFouls() const;

    /** Get the foul history, starting from given foul index
    */
    std::vector<Foul> GetFoulsSince(unsigned index) const;

    void ResetAgentSelection();

    void SelectNextAgent();

    void ClearSelectedPlayers();

protected:
    /** rereads the current soccer script values */
    virtual void UpdateCachedInternal();

    /** set up the reference to the ball and field collider */
    virtual void OnLink();

    /** reset the reference to the ball and field recorder */
    virtual void OnUnlink();

    /** checks if the time for one game half has passed */
    void CheckTime();

    /** updates the RuleAspect during BeforeKickOff mode */
    void UpdateBeforeKickOff();

    /** updates the RuleAspect during KickOff mode */
    void UpdateKickOff(TTeamIndex idx = TI_NONE);

    /** updates the RuleAspect during KickIn mode */
    void UpdateKickIn(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during FreeKick mode */
    void UpdateFreeKick(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during Goal Kick mode */
    void UpdateGoalKick(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during Corner Kick mode */
    void UpdateCornerKick(TTeamIndex ti = TI_NONE);

    /** update the RuleAspect during PlayOn mode */
    void UpdatePlayOn();

    /** update the RuleAspect during Goal mode */
    void UpdateGoal();

    /** update the RuleAspect during Offside mode */
    void UpdateOffside(TTeamIndex ti = TI_NONE);

    /** update the RuleAspect when the match is over */
    void UpdateGameOver();

    /** checks if the ball is not on the playing field and puts it
        back to its last valid position. returns true if the ball left
        the field
    */
    bool CheckBallLeftField();

    /** Checks if the ball is in one of the two goals and scores the
        appropriate team.
        \return true if the ball was in one of the goals.
    */
    bool CheckGoal();

    /** checks if the assistant referee should raise the flag for offside */
    bool CheckOffside();

    /** checks if kickoff taker has kicked the ball again before other players */
    bool CheckKickOffTakerFoul();

    /** moves the ball to pos setting its linear and angular velocity to 0 */
    void MoveBall(const salt::Vector3f& pos);

    /** Move all the players from a team which are too close to pos away
        from pos.
        \param pos the center of the area to be checked
        \param radius the radius of the area to be checked
        \param min_dist the minimum distance players will be moved away from pos
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field. If they would leave the playing field, they are moved
        towards the X axis (the line from the left goal to the right goal).
    */
    void ClearPlayers(const salt::Vector3f& pos, float radius,
                      float min_dist, TTeamIndex idx);
    /** Move all the players from a team inside an rectangular area away
        from that area.
        \param box the rectangular area to be checked
        \param min_dist the minimum distance players will be moved away from box
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field.
    */
    void ClearPlayers(const salt::AABB2& box, float min_dist, TTeamIndex idx);

    /**
     * clear the player before kick off, if the team is the kick off
     * side, the robots can be on his own half and the center circle,
     * otherwise the robots can only be on his own half except the
     * center circle
     *
     * @param idx the team which kick off
     */
    void ClearPlayersBeforeKickOff(TTeamIndex idx);

    /**
     * swaps the side of the teams
     */
    void SwapTeamSides();

    /**
     * Punish agent's foul committed during kickoff
     */
    void PunishKickOffFoul(boost::shared_ptr<oxygen::AgentAspect> agent);

    /** returns true if last kick was happenned in kick off */
    bool WasLastKickFromKickOff(
        boost::shared_ptr<oxygen::AgentAspect> &lastKicker);

protected:
    /** reference to the body node of the Ball */
    boost::shared_ptr<oxygen::RigidBody> mBallBody;

    /** reference to the GameStateAspect */
    CachedPath<GameStateAspect> mGameState;

    /** reference to the BallStateAspect */
    CachedPath<BallStateAspect> mBallState;

    /** the radius of the Ball */
    float mBallRadius;

    /** the length of the pause after a goal */
    float mGoalPauseTime;
    /** the length of the pause after the ball left the field */
    float mKickInPauseTime;
    /** the length of one game half */
    float mHalfTime;
    /** the time we wait before dropping the ball in play modes where only
        one team can touch the ball */
    float mDropBallTime;

    /** the field length (in meters) */
    float mFieldLength;
    /** the field width (in meters) */
    float mFieldWidth;
    /** the goal width (in meters) */
    float mGoalWidth;
    /** the goal height (in meters) */
    float mGoalHeight;
    /** the absolute x coordinate of the goal which ball should pass (in meters) */
    float mGoalBallLineX;

    /** the point on the field where we do the kick in, free kick etc. */
    salt::Vector3f mFreeKickPos;

    /** the distance opponents have to keep during free kicks, kick ins etc. */
    float mFreeKickDist;
    /** the (least) distance opponents will be moved away if they are to close
        during a free kick, kick in and situations like that. */
    float mFreeKickMoveDist;
    /** the distance from the back line the ball should be placed at for a goal kick */
    float mGoalKickDist;

    /** flag if the simulator should do the kick off automatically after the agent */
    bool mAutomaticKickOff;
    /** time to wait until we kick off automatically */
    float mWaitBeforeKickOff;
    /** flag if we want to play only one half of the match */
    bool mSingleHalfTime;
    /** flag if the simulator should quit automatically when the game is over */
    bool mAutomaticQuit;
    /** flag if the side of the teams should be changed in the second half*/
    bool mChangeSidesInSecondHalf;
    /** the time origin from which mWaitBeforeKickOff is calculated */
    float mAutoKickOffTimeOrigin;

    //FCP 2010 - New Parameters (added by FCPortugal for Singapure 2010)
    /** max time player may be sitted or laying down before being repositioned */
    int mNotStandingMaxTime;
    /** max time player may be on the ground before being repositioned */
    int mGroundMaxTime;
    /** max time goalie may be sitted or laying down before being repositioned */
    int mGoalieNotStandingMaxTime;
    /** max time goalie (player number 1) may be on the ground before being repositioned */
    int mGoalieGroundMaxTime;
    /** min dist for second closest of team before being repositioned */
    float mMin2PlDistance;
    /** min dist for third closest of team before being repositioned */
    float mMin3PlDistance;
    /** min dist for closest Opponent to ball in order to use repositions for the second and third player*/
    float mMinOppDistance;
    /** maximum number of players of the defending team that may be inside own penalty area */
    int mMaxPlayersInsideOwnArea;
    /** maximum number of players that may be in a single touch group */
    int mMaxTouchGroupSize;
    /** maximum time allowed for a player to commit a positional foul before being repositioned */
    int mMaxFoulTime;

    /* Useful arrays for dealing with agent state an fouls */
    salt::Vector3f playerPos[12][3];		//Players Positions - not used
    int playerGround[12][3];  		//Time Players are on the ground
    int playerNotStanding[12][3];  	//Time Players are not standing (head up for more than 0.5s)
    int playerInsideOwnArea[12][3];  	//Player is inside own area
    int prevPlayerInsideOwnArea[12][3]; //Player was inside own area last cycle
    int playerStanding[12][3];  	//Time Players are standing
    float distArr[12][3];		//Distance array to ball (left/right team)
    int ordArr[12][3];	  		//Distance order of players (left/right team)
    float distGArr[12][3];		//Distance array to own goal (left/right team)
    int ordGArr[12][3];			//Distance order of players to own goal (left/right team)
    int playerFoulTime[12][3];		//Time player is commiting a positional foul
    EFoulType playerLastFoul[12][3];	//Type of last foul committed by player
    int numPlInsideOwnArea[3]; 		//Number of players inside own area
    int closestPlayer[3]; 		//Closest Player from each team
    float closestPlayerDist[3]; 	//Closest Player distance to ball from each team
    /* FCP 2010 - New Parameters */

    // areas where opponents are not allowed in certain play modes
    /** bounding box for the right half of the field */
    salt::AABB2 mRightHalf;
    /** bounding box for the left half of the field */
    salt::AABB2 mLeftHalf;
    /** bounding box for the right penalty area */
    salt::AABB2 mRightPenaltyArea;
    /** bounding box for the left penalty area */
    salt::AABB2 mLeftPenaltyArea;

    /** say message size max limit */
    int mSayMsgSize;
    /** max distance that player can hear a message */
    float mAudioCutDist;

    /** list of players that are in offside position */
    std::vector<int> mInOffsideLeftPlayers;
    std::vector<int> mInOffsideRightPlayers;
    boost::shared_ptr<oxygen::AgentAspect> mPreLastCollidingAgent;

    /** first colliding player in playon mode */
    bool mFirstCollidingAgent;

    /** if agents don't pass don't worry about offside */
    bool mNotOffside;

    /** if last mode was not playon, clear offside lists*/
    bool mLastModeWasPlayOn;

    /** use offside law */
    bool mUseOffside;

    /** use charging law */
    bool mUseCharging;

    /** minimum player speed to apply charging rules */
    float mChargingMinSpeed;
    /** minimum player distance to ball to apply charging rules */
    float mChargingMinBallDist;
    /** maximum angle between move direction and opponent to apply
     * charging rules */
    float mChargingMaxOppSpeedAngle;
    /** minimum angle between move directions to apply illegal
     * intercept part of charging rules */
    float mChargingIllInterceptMinMutualSpeedAngel;

    /** the time of the kick in the last kick off mode */
    TTime mLastKickOffKickTime;
    /** the player which kicked in the last kick off mode */
    boost::shared_ptr<oxygen::AgentAspect> mLastKickOffTaker;
    /** if kickoff taker should be checked for single kick rule */
    bool mCheckKickOffKickerFoul;
    /** if kickoff taking team has met requirements/rules to score */
    bool mAllowKickOffTeamToScore;
    /** if in penalty shootout mode */
    bool mPenaltyShootout;

    /** complete foul history */
    std::vector<Foul> mFouls;
};

DECLARE_CLASS(SoccerRuleAspect);


#endif // SOCCERRULEASPECT_H
