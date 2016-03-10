#include "troopinfo.h"
using namespace TInfo;
TInfo::TroopList TInfo::FullTroopList({ SoldierType, CatapultType, WallType });
using namespace Attach;
AttachList Attach::FullAttachList({ Torch, BA, Horse, Armor, CataHorse });
BuildList FullBuildList{ SFarm, LFarm, CataFact, TrainCenter, BA_Factory, TorchFactory, Armory, Stable };
BuildList AttachBuilds{ BA_Factory, TorchFactory, Armory, Stable };
BuildList IncomeBuilds{ SFarm, LFarm };
BuildList TrainBuilds{ CataFact, TrainCenter };
