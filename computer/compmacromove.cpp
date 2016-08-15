
#include "computerplayer.h"
#include <headerlib/RangeIterator.h>
#include <headerlib/for_each_extend.hpp>
#include <unordered_map>
#include "help/helpstuff.h"
#include <headerlib/two_d_array.h>
#include "help/iterfns.h"
#include <headerlib/protect_global.hpp>

constexpr int MAX_MOVES = 10;
constexpr int NUM_ITERS = 7;
constexpr double MIN_VALUE = 0.0;

template<class iterator,class fn_ty>
void WrapIter(iterator start,iterator begin,iterator end,fn_ty fn){
    for_each(start,end,fn);
    for_each(begin,start,fn);
}
vector<int> random_order(int first,int last){
    vector<int> series(last - first);
    for(int i = first; i < last; i++){
        series[i] = i;
    }
    random_shuffle(series.begin(),series.end());
    return series;
}

RangeArray<double> ShrinkArray2d(BoardArray<double> & arr,ConstSquare Sq){
    RangeArray<double> res(Sq);
    for(Point P : SquareIterate(Sq)){
        res[P] = arr[P];
    }
    return res;
}
vector<vector<TBVals>> SimpleCompPlayer::ProbsInit(){
    //creates probs andd assigns all to 0.0 or to 1.0 in the spot it is at in the initial move
    vector<vector<TBVals>> probres = ValsInit(0.0);
    vector<TBVals> & zeromove = probres[0];
    for(int PlayN : range(AllPlayers.size())){
        Player * Play = AllPlayers[PlayN];
        TBVals & playvals = zeromove[PlayN];
        for(int TN : random_order(0,Play->Troops.size())){
            Troop * T = Play->Troops[TN];
            playvals.troopvs[TN][T->GetSpot()] = T->Hitpoints;
        }
        for(int BN : range(Play->Buildings.size())){
            Building * B = Play->Buildings[BN];
            for(Point BP : B->Place){
                playvals.buildvs[BN][BP] = 1.0;
            }
        }
    }
    return probres;
}
vector<vector<TBVals>> SimpleCompPlayer::ValsInit(double val){
    //creates values and assigns all to 1.0
    
    vector<vector<TBVals>> res(MAX_MOVES);
    for(vector<TBVals> & movevec : res){
        movevec.resize(AllPlayers.size());
        for(int PlayN : range(AllPlayers.size())){
            movevec[PlayN].troopvs.assign(AllPlayers[PlayN]->Troops.size(),BoardArray<double>(val));
            movevec[PlayN].buildvs.assign(AllPlayers[PlayN]->Buildings.size(),BoardArray<double>(val));
        }
    }
    return res;
}
void probs_to_pos_vals(vector<BoardArray<double>> & probs){
    //ensure that pos_vals never exceed 0 where the thing cannot move
    //this may mean changing ValInit.
    for(BoardArray<double> & arr : probs){
        //????
    }
}
void calc_future_vals(vector<BoardArray<double>> & pos_vals,vector<BoardArray<double>> & future_vals,int MoveRange){
    vector<BoardArray<double>> move_to_val(MAX_MOVES,MIN_VALUE);
    future_vals.resize(MAX_MOVES);
    
    move_to_val[0] = pos_vals[0];
    for(int M : range(1,MAX_MOVES)){
        for(Point CurP : BoardIterate()){
            double max_v = MIN_VALUE;
            for(Point PrevP : SquareIterate(CurP,MoveRange)){
                max_v = max(max_v,move_to_val[M-1][PrevP]);
            }
            move_to_val[M][CurP] = max_v + pos_vals[M][CurP];
        }
    }
    vector<BoardArray<double>> move_path_val(MAX_MOVES,MIN_VALUE);
    move_path_val[MAX_MOVES-1] = move_to_val[MAX_MOVES-1];
    future_vals[MAX_MOVES-1] = pos_vals[MAX_MOVES-1];
    
    for(int M : range(MAX_MOVES-2,-1,-1)){
        for(Point CurP : BoardIterate()){
            double max_v = MIN_VALUE;
            for(Point NextP : SquareIterate(CurP,MoveRange)){
                max_v = max(max_v,move_path_val[M+1][NextP]);
            }
            move_path_val[M][CurP] = max_v - (move_to_val[M+1][CurP] - pos_vals[M+1][CurP]) + move_to_val[M][CurP];
            
            future_vals[M][CurP] = move_path_val[M][CurP] + pos_vals[M][CurP] -  move_to_val[M][CurP];
        }
    }
}
vector<BoardArray<double>> GetTroopMoveValVec(vector<vector<TBVals>> & AllItems,int PlayNum,int TNum){
    //makes a vector of move items with troop num and play num fixed
    vector<BoardArray<double>> movevec(MAX_MOVES);
    for(int M : range(MAX_MOVES)){
        movevec[M] = AllItems[M][PlayNum].troopvs[TNum];
    }
    return movevec;
}
void place_val_vec_into(vector<vector<TBVals>> & AllItems,vector<BoardArray<double>> & val_vec,int PlayNum,int TNum){
    //places vector of move items with troop num and play num fixed into all_items
    for(int M : range(MAX_MOVES)){
        AllItems[M][PlayNum].troopvs[TNum] = val_vec[M];
    }
}
vector<RangeArray<double>> SimpleCompPlayer::GetInteractingPaths(){
    /*
    starting with a constant value(of 1), the values of troop movement are calculated. The values divided by the
    total value is taken to be the percentage chance of the troop being on that square. This is then used to affect the
    chances of buildings being there there, of land being dominated by differnt troops, and of buildings existing. These
    affectings are easily translated to be the values of the troop moving there. This is then taken to be the
    */
    //move,player,(Troop data,Building data)
    
    const double iter0val = 1.0;//arbitrary non-zero value
    vector<vector<TBVals>> AllVals = ValsInit(iter0val);
    
    for(int Iter : range(NUM_ITERS)){
        vector<vector<TBVals>> AllProbs = ProbsInit();
        for(int Move : range(MAX_MOVES)){
            WrapIter(AllPlayers.begin() + this->PlayerNum,AllPlayers.begin(),AllPlayers.end(),[&](Player * Play){
                //performs attacks (decreases on probabilty) on other players probabilty values as it calculates its own probabilties.
                //note that final move will not be attacked in this way, so it will be flawed.
                
                // sequencial algorithm, troop ordering matters, but there is currently
                // no ordering mechanism to optimize this process, so it is randomized to reduce impact on outcome
                
                for(int TN : random_order(0,Play->Troops.size())){
                    
                }
                //ordering does not affect buildings, they can be done in any order
                for(int BN : range(Play->Buildings.size())){
                    
                }
            }); 
        }
        #ifdef Debug_Macro_Move 
        VecTVals.probvals[0].push_back(AllProbs);
        VecTVals.probvals[1].push_back(AllProbs);
        #endif
        //calculates next iteration's values from old values and probabilities
        //vector<vector<TBVals>> OldVals = AllVals;
        //order does not matter at all here.
        for(Player * Play : AllPlayers){
            for(int TN : range(Play->Troops.size())){
                vector<BoardArray<double>> pos_vals = GetTroopMoveValVec(AllProbs,Play->PlayerNum,TN);
                probs_to_pos_vals(pos_vals);
                vector<BoardArray<double>> future_vals;
                calc_future_vals(pos_vals,future_vals,Play->Troops[TN]->MovementPoints);
                place_val_vec_into(AllVals,future_vals,Play->PlayerNum,TN);
            }
            for(int BN : range(Play->Buildings.size())){
                
            }
        }
    }
    vector<BoardArray<double>> my_tvals = AllVals[0][this->PlayerNum].troopvs;
    vector<RangeArray<double>> retvals(my_tvals.size());
    for(int TN : range(my_tvals.size())){
        Troop * T = Troops[TN];
        retvals[TN] = ShrinkArray2d(my_tvals[TN],ConstSquare(T->GetSpot(),T->MovementPoints));
    }
    return retvals;
}


















struct Path{
    Point P;

    Path * LastPath;
    double CumVal;

    Path * NextPath;
    double PathVal;

    Path(){ LastPath = NULL; NextPath = NULL; CumVal = -10e50; PathVal = -10e50; }
    Path(Point InP, double AddVal, Path * InLastPath){
        P = InP;
        LastPath = InLastPath;
        CumVal = AddVal;
        NextPath = NULL;
        if (LastPath != NULL)
            CumVal += LastPath->CumVal;
    };
    void PushPath(vector<Point> & OutPath){
        OutPath.push_back(P);
        if (NextPath != NULL)
            NextPath->PushPath(OutPath);
    }
};
int GetAdjRange(int PlaceNum, int MoveRange){
    return PlaceNum == 0 ? 1 : (PlaceNum + MoveRange - 1) / MoveRange;
}
double GetValMoveAdjust(int MoveNum){
    return 1.0 / Square(MoveNum);
}
BoardArray<double> SimpleCompPlayer::GetValuesOfTroopSquares(Troop * ThisT, Attach::AttachVals TroopAttachVals, Modifier Mod){
    BoardArray<double> Vals(0);
    int Range = ThisT->Range;

    auto AddValToRange = [&](double AddVal, Point P, int Range){
        //this function take the value that needs to be added on and spreads it over the board
        //gets the total value of the iteration do it can spread it without the total value increasing
        //double TotDiv = 0;
        //for(Point IterP : SquareIterate(P,Range)){
        //	TotDiv += 1 - GetBoardDistance(P,IterP) / double(Range);
        //}
        ////adds on the appropriate parts of the values to each part of the board
        //for(Point IterP : SquareIterate(P,Range)){
        //	int Dis = GetBoardDistance(P, IterP);
        //	double Div = (1 - Dis / double(Range)) / TotDiv;

        //	Vals[IterP] += AddVal * Div;
        //}
        for (Point P : SquareIterate(P, Range)){
            Vals[P] += AddVal;
        }
    };
    //this puts down the value of the enemy stuff on the board
    for (Point BP : BoardIterate()){
        if (!BlankPoint(BP) && GetPlayer(BP)->TeamNum != this->TeamNum)
            AddValToRange(GetValOfAttack(BP, ThisT) * Mod.AttackMod, BP, Range);
    }
    //get the value that is dependant on threat
    for (Point P : BoardIterate()){
        SquareThreat & ThisThreat = Threat[P];
        //change when attack vs defense actually becomes an issue
        double ValOfThreat = -(ThisT->GetValue() * ThisThreat.TThreat) * Mod.ThreatMod;
        Vals[P] += ValOfThreat;
    }
    //get the value that is dependant on land ownership
    map<EPlayer *, double> LandValue;
    for (EPlayer * Play : AllPlayers)
        LandValue[Play] = GetValOfDom(Play) * Mod.LandMod;

    for (Point P : BoardIterate()){
        int InfluenceToDom;
        double ThisValOfDom = LandValue[this];
        if (PlayerDom[P].Player >= 0){
            EPlayer * OccPlay = AllPlayers[PlayerDom[P].Player];

            if (OccPlay != NULL && PlayerDom[P].Influence == MaximumDomination){
                double OccValOfDom = LandValue[OccPlay];//if OccPlay == this, then the values are the same and cancel out to 0
                if (OnSameTeam(this, OccPlay))
                    ThisValOfDom -= OccValOfDom;
                else
                    ThisValOfDom += OccValOfDom;
            }
            if (OccPlay->PlayerNum == this->PlayerNum)
                InfluenceToDom = MaximumDomination - PlayerDom[P].Influence;
            else//adresses the nature of taking away enemies terretory twice as fast as making one's own
                InfluenceToDom = MaximumDomination + (PlayerDom[P].Influence / 2);
        }
        else{//if there is no one there, the player needs MaximumDomination to overcome it
            InfluenceToDom = MaximumDomination;
        }
        //if there is no need to dominate, the added value is zero and is needed to stop 0/0div
        if (InfluenceToDom == 0)
            continue;
        for (Point IterP : SquareIterate(P, Range)){
            int ThisInfluence = GetDominationOfTroop(Range, GetBoardDistance(IterP, P));
            ThisInfluence = min(ThisInfluence, InfluenceToDom);
            Vals[P] += (ThisValOfDom * ThisInfluence) / InfluenceToDom;
        }
    }
    //gets the value of defending buildings
    for (Building * B : Buildings){
        double BSpotVal = B->GetCost() / double(B->GetSize());
        for (Point BP : B->Place){
            if (FutureThreat[BP].BThreat > 0){
                //goes after the troops that could attack this building in futuremovesahead
                for (Troop * EnTroop : FutureThreat[BP].ThreatTroops){
                    if (EnTroop->TAttackType == TroopAndBuild){
                        //value is related to how far away the threatening troop is
                        int EnMovesToToBuild = GetAdjRange((GetBoardDistance(EnTroop->GetSpot(), BP) - EnTroop->Range), EnTroop->MovementPoints); //doesn't account for blocking at all
                        //this value is currently far too large!!!
                        AddValToRange(Mod.DefendMod * BSpotVal / EnMovesToToBuild, EnTroop->GetSpot(), Range);
                    }
                }
            }
        }
    }
    //adds in the values of the group being close to a center which it needs to buy attachments from
    for (auto AttachPair : TroopAttachVals){
        double AttachVal = AttachPair.Val;
        Attach::Attachment Attc = AttachPair.Info;

        if (ThisT->HasAttach(Attc) == true)//buying the attachment has no value if the troop already has it
            continue;

        if (AttachVal < 0.1)//skips the next part if negligable value
            continue;
        //this creates a map of the values where the group can buy the attachment it wants
        //warning!! it will double count same types of Attachment buildings(although this is not necessarily bad)
        for (Building * B : Buildings){
            if (B->Type == AttBuildReqs(Attc)){
                B->IterOverBuildFromDis(PlayerOcc, Attach::AttachBuyRange, [&](Point BuyP){
                    Vals[BuyP] += AttachVal;
                });
            }
        }
    }
    return Vals;
}
//LandVals is a double represenation of the influence part of PlayerDom
double GetOneSidedChanceToWin(Troop * Attack, Troop * Defend){
    if (Attack->AttackValue == 0 || !Defend->AttemptAttack(Attack))
        return 10e-50;

    int HitPerAttack = RoundUpRatio(Attack->Hitpoints, Defend->AttackValue);

    int RangeDif = Attack->Range - Defend->Range;

    int MoveDif = Attack->MovementPoints - Defend->MovementPoints;

    //if this is the case, the defending troop can simply attack and retreat over and over again, and so is in a very solid position
    if (RangeDif <  MoveDif - Defend->MovementPoints)
        return 10e-50;

    int MovesToCloseRange =
        (RangeDif > MoveDif);

    return HitPerAttack * (1 + 2 * MovesToCloseRange);
}
double GetChanceToWin(Troop * Attack, Troop * Defend){
    double AttackWin = GetOneSidedChanceToWin(Attack, Defend);
    double DefWin = GetOneSidedChanceToWin(Defend, Attack);
    return AttackWin / (AttackWin + DefWin);
}
void AffectTroopVals(BoardArray<double> & TVals, Troop * Attack,Troop * Defend){
    //here TVals is associated with Attack

}
BoardArray<uint16_t> FindMoveDistances(Point Start, int MoveRange, int StopDis){
    //This section makes an Array2d that represents the Board in terms of the minimum number of moves it takes to get to each square
    BoardArray<uint16_t> ShortMoves(uint16_t(0));

    vector<Point> CurSpots;
    vector<Point> PrevSpots;
    PrevSpots.push_back(Start);
    int MoveTurn = 1;
    ShortMoves[Start] = MoveTurn;
    while (PrevSpots.size() > 0 && MoveTurn <= StopDis + 1){
        CurSpots.clear();
        for (Point PrevP : PrevSpots){
            for (Point CheckP : SquareIterate(PrevP, 1)){
                if (BlankPoint(CheckP) && ShortMoves[CheckP] == 0){
                    ShortMoves[PrevP] = MoveTurn;
                    CurSpots.push_back(CheckP);
                }
            }
        }
        MoveTurn++;
        PrevSpots = CurSpots;
    }
    //converts it from squares away to moves away
    for (uint16_t & Moves : ShortMoves){
        Moves = (Moves + MoveRange - 1) / MoveRange;
    }
    return ShortMoves;
}
void AttachPrevPaths(BoardArray<Path> & PrevPaths, BoardArray<Path> & CurPaths, double ValMoveAdjust,BoardArray<double> & Vals){
    for (Point CurP : BoardIterate()){
        if (!BlankPoint(CurP))
            continue;
        Path * BestPrevPath = &PrevPaths[CurP];//sets an arbitrary starting point
        for (Point PrevP : SquareIterate(CurP, 1)){
            if (PrevPaths[PrevP].CumVal > BestPrevPath->CumVal)
                BestPrevPath = &PrevPaths[PrevP];
        }
        if (BestPrevPath->CumVal > -1000000.0)//if there is an active square that works
            CurPaths[CurP] = Path(CurP, Vals[CurP] * ValMoveAdjust, BestPrevPath);//CurPath is not yet initialized
    }
}
void AttachCurPaths(BoardArray<Path> & CurPaths, BoardArray<Path> & NextPaths,Point Start){
    for (Point CurP : BoardIterate()){
        if (!BlankPoint(CurP))
            continue;
        Path * BestNextPath = &NextPaths[CurP];//sets an arbitrary starting point
        for (Point NextP : SquareIterate(CurP, 1)){
            if (NextPaths[NextP].PathVal > BestNextPath->PathVal)
                BestNextPath = &NextPaths[NextP];
        }
        if (BestNextPath->LastPath != NULL || BestNextPath->P == Start){//if there is an active square that works
            Path & CurPath = CurPaths[CurP];
            CurPath.NextPath = BestNextPath;
            //Since the PathVal = the best ones added up, this takes the PathVal - BestOne + ThisOne to find the value of this path
            CurPath.PathVal = BestNextPath->PathVal - (BestNextPath->LastPath->CumVal - CurPath.CumVal);
        }
    }
}
PartialRangeArray<ValInfo<vector<Point>>> ConvertToMovePaths(vector<BoardArray<Path>> & Paths, Point Start, int MoveRange){
    //returns the useful values in a more sensible container, a blocked out 2d array around the MoveRange
    PartialRangeArray<ValInfo<vector<Point>>> PathVals(Start, MoveRange);

    BoardArray<uint16_t> PathNums = FindMoveDistances(Start, MoveRange, MoveRange);

    int MoveNum = MoveRange;
    BoardArray<Path> & MovePaths = Paths[MoveNum];
    for (Point MoveP : BoardIterate()){
        if (PathNums[MoveP] <= 0)
            continue;

        PathVals.SetExist(MoveP, true);

        ValInfo<vector<Point>> & ThisVal = PathVals[MoveP];
        ThisVal.Val = MovePaths[MoveP].PathVal;
        MovePaths[MoveP].PushPath(ThisVal.Info);//ThisVal = the path
    }
    return PathVals;
}
void SimpleCompPlayer::RemoveMoveableTroops(){
    for (EPlayer * Play : AllPlayers)
        for (Troop * T : Play->Troops)
            if (T->MovementPoints > 0)
                PlayerOcc[T->GetSpot()] = -1;
}
PartialRangeArray<ValInfo<vector<Point>>> SimpleCompPlayer::GetPaths(Point Start, int MoveRange,BoardArray<double> Vals){

    const int MaxRange = max(BoardSizeX, BoardSizeY);
    const int MaxMoves = RoundUpRatio(MaxRange, MoveRange);
    if (MoveRange == 0){
        throw "Getpaths doesn't work for range 0 troops!";
    }

    auto Protectkey = Protect(PlayerOcc);
    RemoveMoveableTroops();

    if (!BlankPoint(Start))
        throw "Start needs to be empty for GetPaths to work properly";

    vector<BoardArray<Path>> Paths(MaxRange);//the paths are to their neighbors that led to them

    /*
    This creates each new layer in a way that the next square looks back at the squares around it
    and finds the one with the greatest path so far. This way, each square is part of the best path it can
    be on.
    */
    Paths[0][Start] = Path(Start,Vals[Start], NULL);
    for (int MoveNum : range(1, MaxRange)){
        double ValMoveAdjust = GetValMoveAdjust(GetAdjRange(MoveNum, MoveRange));
        AttachPrevPaths(Paths[MoveNum - 1], Paths[MoveNum], ValMoveAdjust, Vals);
    }

    //sets the furthest outs PathVals (here , the PathVal == CumVal by definition)
    for (Path & LastPath : Paths[MaxRange - 1]){
        LastPath.PathVal = LastPath.CumVal;
    }
    /*
    With the assumption that the furthest out PathVal will contain the best paths (which may be false)
    this goes through backwards to find the best path that it can link to
    */
    for (int MoveNum : range(MaxRange - 1, 0, -1)){
        AttachCurPaths(Paths[MoveNum - 1], Paths[MoveNum], Start);
    }

    auto RetVal = ConvertToMovePaths(Paths, Start, MoveRange);
    return RetVal;
}

void MakePathVals(vector<BoardArray<double> *> & InVals, vector<BoardArray<double> *> & OutVals, Point Start, int MoveRange, int Moves){
    int Futures = (Moves - 1) * MoveRange + 1;//so that rounding up never gets passed the last movevector<Array2d<Path>> Paths(Moves * MoveRange);
    vector<BoardArray<Path>> Paths(Futures);

    Paths[0][Start] = Path(Start, (*InVals[0])[Start], NULL);

    for (int SM : range(1, Futures)){
        int M = GetAdjRange(SM, MoveRange);
        AttachPrevPaths(Paths[SM - 1], Paths[SM], GetValMoveAdjust(M), *InVals[M]);//has constant value over movenum
    }
    for (Path & LastPath : Paths[Futures - 1]){
        LastPath.PathVal = LastPath.CumVal;
    }
    for (int SM : range(Futures - 1, 0, -1)){
        AttachCurPaths(Paths[SM - 1], Paths[SM], Start);//has constant value over movenum
    }

    for (int MoveNum : range(Moves)){
        int PNum = MoveRange * MoveNum;

        for_each_range(Paths[PNum], *OutVals[MoveNum], [&](Path & Pa, double & Val){
            Val = (Pa.PathVal > -10e7) * Pa.PathVal;
        });
    }
}
