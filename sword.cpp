#ifndef _definition_h_
#include "defs.h"
#define _definition_h_
#endif


#define MadBearDamage         1
#define MoonBringerDamage     1.5
#define ElfDamages            4.5
#define SaxonDamage           6.5
#define TrollDamage           8.5
#define MadBearBounty         100
#define MoonBringerBounty     150
#define ElfBounty             450
#define SaxonBounty           650
#define TrollBounty           850

bool    hadExcalibur = false;
bool    beatUltimecia = false;
int     winCount = 0;
int     loseCount = 0;
int     eventIndex = 0;
//Check for having any treasure
bool    hadPaladinShield = false;
bool    hadLancelotSpear = false;
bool    hadGuinevereHair = false;
//Status variable
bool beingPoisoned = false;
int poisonTime = 0; 


enum EVENT{
    MadBear = 1,
    AmazonMoonBringer,
    Elf,
    Saxon,
    Troll,
    TornBery,
    QueenOfCards,
    NinaDeRings,
    DurianGarden,
    Atidode,
    Odin,
    Merlin,
    OmegaWeapon,
    Hades,
    ScarletHakama,
    LockedDoor,
    PaladinShield = 95,
    LancelotSpear,
    GuinevereHair,
    Excalibur,
    Ultimecia
};

//Clear all unfavorable status
void clearUnfavorableStatus() {
    beingPoisoned = 0;
    poisonTime = 0;
}

//Event handle for event 1 to 5
void fight(knight& theKnight, int event, float opponentDamage, int opponentBounty) {
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    int   damage = opponentDamage * levelO * 10;
    std::clog << "Compare level: " << theKnight.level << " " << levelO << '\n';
    if ( ~hadExcalibur || theKnight.level < levelO) {
        theKnight.HP -= damage;
        loseCount++;
        if (theKnight.HP <= 0) {
            callPhoenix(theKnight, maxHP);
            clearUnfavorableStatus();
        }
    }
    else {
        if (beingPoisoned) {
            theKnight.HP -= damage;
            if (theKnight.HP <= 0) {
                callPhoenix(theKnight, maxHP);
                clearUnfavorableStatus();
            }
        }
        theKnight.gil += opponentBounty;
        winCount++;
    }
    std::clog << "count " << winCount << " " << loseCount << '\n';
}
//Event handel for event 6
void dealWithTornBery(knight& theKnight, int event){
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    if ( ~hadExcalibur || theKnight.level < levelO) {
        if (theKnight.antidote) theKnight.antidote--;
        else {
            beingPoisoned = true;
            poisonTime = 6;
        }
        loseCount++;
    }
    else {
        theKnight.level = (theKnight.level < 10) ? (theKnight.level + 1) : 10;  
        winCount++;
    }
}
//Check for current status;
void endOfEventCheck(){
    if (beingPoisoned) {
        poisonTime--;
        if (poisonTime = 0) beingPoisoned = false;
    }
}


report*  walkthrough (knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
    report* pReturn;
    int bFlag;
    //fighting for the existence of mankind here
    while (true) {
        // std::clog << "Enter castle" << '\n'; 
        // std::clog << "Data: " << theKnight.HP << " " << theKnight.level << " " << theKnight.gil << " " << theKnight.antidote << '\n';
        for (int i = 0; i < nCastle ; i++) {  
            for (int j = 0; j < arrCastle[i].nEvent; j++) {
                eventIndex++;
                if (beatUltimecia) {
                    break;
                }
                else if ((nPetal == 0) && !beatUltimecia) {
                    break;
                }
                switch (arrCastle[i].arrEvent[j]) {
                    case Ultimecia: 
                        if (hadExcalibur == true) {
                            if (beingPoisoned) theKnight.HP = theKnight.HP < 3 ? 1 : theKnight.HP / 3;
                            winCount++;
                            beatUltimecia = true;
                        } else {
                            theKnight.HP = theKnight.HP < 3 ? 1 : theKnight.HP / 3;
                            loseCount++;
                        }
                        break;
                    case Excalibur:
                        if (hadPaladinShield && hadLancelotSpear && hadGuinevereHair) hadExcalibur = true;
                        break;
                    case MadBear:
                    std::clog << "reach" << '\n';
                        fight(theKnight, eventIndex, MadBearDamage, MadBearBounty);
                        break;
                    case AmazonMoonBringer:
                        fight(theKnight, eventIndex, MoonBringerDamage, MoonBringerBounty);
                        break;
                    case Elf:
                        fight(theKnight, eventIndex, ElfDamages, ElfBounty);
                        break;
                    case Saxon:
                        fight(theKnight, eventIndex, SaxonDamage, SaxonBounty);
                        break;
                    case Troll:
                        fight(theKnight, eventIndex, TrollDamage, TrollBounty);
                        break;
                    case TornBery:
                        if (beingPoisoned) break;
                        else dealWithTornBery(theKnight, eventIndex);
                        std::clog << "Status check: " << beingPoisoned << '\n';
                        std::clog << "Data: " << theKnight.HP << " " << theKnight.level << " " << theKnight.gil << " " << theKnight.antidote << '\n';
                        break;
                    case PaladinShield:
                        hadPaladinShield = true;
                        break;
                    case LancelotSpear: 
                        hadLancelotSpear = true;
                        break;
                    case GuinevereHair:
                        hadGuinevereHair = true;
                        break;
                    default:
                        break;
                }
                endOfEventCheck();
                --nPetal;
            }
            if (!beatUltimecia) {
                theKnight.level = (theKnight.level == 10) ? 10 : theKnight.level + 1;
                maxHP = ((maxHP + 100) > 999) ? 999 : maxHP + 100;
            }
        }
        if (beatUltimecia) {
            bFlag = 1;
            break;
        }
        else if ((nPetal == 0) && !beatUltimecia) {
            bFlag = 0;
            break;
        } 
    }
    // success or failure?
    //if (bFlag) bFlag = 1;
    pReturn = (bFlag)? new report : NULL;
    
    if (pReturn != nullptr) {
        pReturn->nPetal = nPetal;
        pReturn->nWin = winCount;
        pReturn->nLose = loseCount;
    }
    return pReturn;
}
