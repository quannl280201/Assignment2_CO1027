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
bool    hadPaladinShield = false;
bool    hadLancelotSpear = false;
bool    hadGuinevereHair = false;

enum EVENT{
    MadBear,
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

void fight(knight& theKnight, int event, float opponentDamage, int opponentBounty) {
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    if ( ~hadExcalibur || theKnight.level < levelO) {
        int   damage = opponentDamage * levelO * 10;
        theKnight.HP -= damage;
        loseCount++;
        if (theKnight.HP <= 0) callPhoenix(theKnight, maxHP);
    }
    else {
        theKnight.gil += opponentBounty;
        winCount++;
    }
}

report*  walkthrough (knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
    report* pReturn;
    int bFlag;
    //fighting for the existence of mankind here
    for (int i = 0; i < nCastle; i++) { 
        if ((nPetal == 0) && ~beatUltimecia) {
            bFlag = 0;
            break;
        }
        for (int j = 0; j < arrCastle[i].nEvent; j++) {
            eventIndex++;
            if ((nPetal == 0) && ~beatUltimecia) {
                bFlag = 0;
                break;
            }
            switch (arrCastle[i].arrEvent[j]) {
                case Ultimecia: 
                    if (hadExcalibur == true) {
                    beatUltimecia = true;
                    }
                    break;
                case Excalibur:
                    if (hadPaladinShield && hadLancelotSpear && hadGuinevereHair) hadExcalibur = true;
                    break;
                case MadBear:
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
            --nPetal;
        }
        theKnight.level = (theKnight.level == 10) ? 10 : theKnight.level + 1;
        maxHP = ((maxHP + 100) > 999) ? 999 : maxHP + 100;
    }
    
    // success or failure?	
    pReturn = (bFlag)? new report : NULL;
    if (pReturn != nullptr) {
        pReturn->nPetal = nPetal;
        pReturn->nWin = winCount;
        pReturn->nLose = loseCount;
    }
    return pReturn;
}
