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

bool    hadExcalibur            = false;
bool    beatUltimecia           = false;
int     winCount                = 0;
int     loseCount               = 0;
int     eventIndex              = 0;
/*Special Character*/
bool    isArthur                = false;
bool    isLancelot              = false;
bool    isGuinevere             = false;
bool    isDragonKnight          = false;
bool    isPaladin               = false;
/*Check for having any treasure*/
bool    hadPaladinShield        = false;
bool    hadLancelotSpear        = false;
bool    hadGuinevereHair        = false;
/*Status variable*/
bool    beingPoisoned           = false;
int     poisonTime              = 0; 
bool    hadOdinHelp             = false;
int     odinHelpTime            = 0;
bool    odinIsDead              = false;
bool    hadBeatOmega            = false;
int    getOverChallenge         = 0;
/*Special Items*/
bool    hadLionHeart            = false;
int     lionHeartTime           = 6;
bool    hadMythril              = false;
bool    hadScarletHakama        = false;



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
//Chech for Eternal Love
bool hadEternalLove(){
    if (hadGuinevereHair && hadLancelotSpear && (!hadExcalibur)) return true;
    else if (isArthur && hadGuinevereHair) return true; 
    else if (isLancelot && hadGuinevereHair) return true;
    else if (isGuinevere && hadLancelotSpear) return true;
    else return false;
}
//Event handle for event 1 to 5
void fight(knight& theKnight, int event, float opponentDamage, int opponentBounty) {
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    int   damage = opponentDamage * levelO * 10;
    if (isArthur || isLancelot || isPaladin || theKnight.level >= levelO || hadOdinHelp) {
        if (beingPoisoned && !hadMythril) {
            theKnight.HP -= damage;
            if (theKnight.HP <= 0) {
                callPhoenix(theKnight, maxHP);
                clearUnfavorableStatus();
            }
        }
        theKnight.gil += opponentBounty;
        winCount++;   
    }
    else {
        if (isGuinevere && opponentDamage == MoonBringerDamage) return;
        if(beingPoisoned) theKnight.HP -= damage;
        if (!hadMythril)theKnight.HP -= damage;
        if (theKnight.HP <= 0) {
            callPhoenix(theKnight, maxHP);
            clearUnfavorableStatus();
        }
        loseCount++;
    }
}
//Event handel for event 6
void dealWithTornBery(knight& theKnight, int event){
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    if (isArthur || isLancelot || theKnight.level >= levelO || hadOdinHelp) {
        theKnight.level = (theKnight.level < 10) ? (theKnight.level + 1) : 10;  
        winCount++;
    }
    else {
        if (theKnight.antidote) theKnight.antidote--;
        else if (!isPaladin){
            beingPoisoned = true;
            poisonTime = 6;
        }
        loseCount++;
    }
}

void dealWithQueenOfCards(knight &theKnight, int event){
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    if (isArthur || isLancelot || theKnight.level >= levelO || hadOdinHelp ) {
        theKnight.gil *= 2;
        winCount++;
    }
    else {
        if (!hadScarletHakama && !isGuinevere) theKnight.gil /= 2;
        loseCount++;
    }
}

bool isFriendlyPair(int HP, int gil) {
    int sum_1 = 0;
    int sum_2 =0;
    for (int i = 1; i < HP / 2; i++) if (HP % i == 0) sum_1 += i;
    for (int i = 1; i < gil / 2; i++) if (gil % i == 0) sum_2 += i;
    float abundance_1  = (float)(sum_1 / HP);
    float abundance_2  = (float)(sum_2 / gil);
    if (abundance_1 == abundance_2) return true;
    else return false;

}

void tradeWithNina(knight& theKnight){
    if (isFriendlyPair(theKnight.HP, theKnight.gil) || isPaladin) {
        if (beingPoisoned) {
            beingPoisoned = false;
            poisonTime = 0;
        }
        theKnight.HP = maxHP;
        hadLionHeart = true;
        lionHeartTime = isPaladin ? 0 : 6;
        
    }
    else if (isGuinevere) {
        if (beingPoisoned) {
            beingPoisoned = false;
            poisonTime = 0;
        }
        theKnight.HP = maxHP;
        theKnight.gil = theKnight.gil > 949 ? 999 : theKnight.gil + 50;
    }
    else {
        if (theKnight.gil < 50) return;
        if (beingPoisoned) {
            beingPoisoned = false;
            poisonTime = 0;
            if (!hadScarletHakama) theKnight.gil -=50;
        }
        if (theKnight.gil) {
            int tradeRatio = maxHP - theKnight.HP;
            if (tradeRatio > theKnight.gil) {
                theKnight.HP += theKnight.gil;
                if (!hadScarletHakama) theKnight.gil = 0;
            }
            else {
                theKnight.HP = maxHP;
                if (!hadScarletHakama) theKnight.gil -= tradeRatio;
            }
        }
    }
}
void merlinHelp(knight &theKnight){
    if (beingPoisoned) {
        beingPoisoned = false;
        poisonTime = 0;
    }
    int tempLevel = theKnight.level;
    theKnight.level = (theKnight.level == 10) ? 10 : theKnight.level + 1;
    maxHP = (tempLevel == theKnight.level) ? maxHP : ((maxHP < 900) ? maxHP + 100 : 999);
    theKnight.HP = maxHP;
}
void fightWithOmega(knight &theKnight) {
    if (theKnight.level == 10 && hadExcalibur || isDragonKnight && hadLionHeart) {
        int HPincrease = 10 - theKnight.level;
        theKnight.level = 10;
        maxHP = (maxHP + HPincrease * 100) > 999 ? 999 : maxHP + HPincrease * 100;
        theKnight.gil = 999;
        winCount++;
    } else {
        if (!hadMythril) {
            theKnight.HP = 0;
            callPhoenix(theKnight, maxHP);
            clearUnfavorableStatus();
        }
        loseCount++;
    }
}
void fightWithHades(knight &theKnight, int event){
    if (hadOdinHelp && !isDragonKnight) {
        hadOdinHelp = false;
        odinHelpTime = 0;
        odinIsDead = true;
    }
    else if (hadOdinHelp && isDragonKnight){
        winCount++;
        hadMythril = true;
        return;
    }
    if (hadEternalLove()) {
        hadMythril = true;
        winCount++;
        return;
    }
    int   b = event % 10;
    int   levelO = event > 6 ? (b > 5 ? b : 5) : b;
    if (theKnight.level >= levelO) {
        hadMythril = true;
        winCount++;
    }
    else {
        if (!hadMythril) {
            theKnight.HP = 0;
            callPhoenix(theKnight, maxHP);
        }
        loseCount++;
    }
    return;
}

//Check for current status;
void endOfEventCheck(){
    if (beingPoisoned) {
        poisonTime--;
        if (poisonTime = 0) beingPoisoned = false;
    }
    if (hadOdinHelp) odinHelpTime--;
    if (hadLionHeart) lionHeartTime--;
}

bool isPaladincheck(int hp){
	int m = hp / 2;
	for (int i = 2; i < m; i++) {
		if (hp % i == 0) return false;
	}
	return true;
}
bool isDragonKnightCheck(int HP){ 
    for (int i = 1; i <= HP / 3; i++) { 
        for (int j = i + 1; j <= HP / 2; j++) { 
            int k = HP - i - j; 
            if (i * i + j * j == k * k) return true;             
        }
    }
    return false;
}

report*  walkthrough (knight& theKnight, castle arrCastle[], int nCastle, int mode, int nPetal)
{
    report* pReturn;
    int bFlag;
    if (theKnight.HP == 999) isArthur = true;
    else if (theKnight.HP == 888) {
        isLancelot = true;
        hadLancelotSpear = true;
    }
    else if (theKnight.HP == 777) {
        isGuinevere = true;
        hadGuinevereHair = true;
    }
    else if (isPaladincheck(theKnight.HP)) {
        isPaladin = true;
        hadPaladinShield = true;
    }
    else if (isDragonKnightCheck(theKnight.HP)) isDragonKnight = true;
    //fighting for the existence of mankind here
    while (true) {
        for (int i = 0; i < nCastle ; i++) {
            eventIndex = 0;  
            for (int j = 0; j < arrCastle[i].nEvent; j++) {
                eventIndex++;
                if (beatUltimecia) {
                    break;
                }
                else if (!isArthur && (nPetal == 0) && !beatUltimecia) {
                    break;
                }
                switch (arrCastle[i].arrEvent[j]) {
                    case Ultimecia: 
                        if (hadExcalibur == true) {
                            if (beingPoisoned) theKnight.HP = theKnight.HP < 3 ? 1 : theKnight.HP / 3;
                            winCount++;
                            beatUltimecia = true;
                        } 
                        else {
                            if (isGuinevere) {
                                loseCount++;
                                break;
                            }
                            theKnight.HP = theKnight.HP < 3 ? 1 : theKnight.HP / 3;
                            loseCount++;
                        }
                        break;
                    case Excalibur:
                        if (isArthur || hadPaladinShield && hadLancelotSpear && hadGuinevereHair) hadExcalibur = true;
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
                    case TornBery:
                        if (beingPoisoned) break;
                        else dealWithTornBery(theKnight, eventIndex);
                        break;
                    case QueenOfCards:
                        dealWithQueenOfCards(theKnight, eventIndex);
                        break;
                    case NinaDeRings:
                        tradeWithNina(theKnight);
                        break;
                    case DurianGarden:
                        if (hadScarletHakama) nPetal = 99;
                        if (beingPoisoned) {
                            beingPoisoned = false;
                            poisonTime = 0;
                        }
                        theKnight.HP = maxHP;
                        nPetal  = (nPetal > 94) ? 99 : nPetal + 5;
                        break;
                    case Atidode:
                        if (beingPoisoned) {
                            beingPoisoned = false;
                            poisonTime = 0;
                        }else theKnight.antidote++;
                        break;
                    case Odin:
                        if (odinIsDead) break;
                        hadOdinHelp = true;
                        odinHelpTime = 6;
                        break;
                    case Merlin:
                        merlinHelp(theKnight);
                        break;
                    case OmegaWeapon:
                        if (!hadBeatOmega) fightWithOmega(theKnight);
                        break;
                    case Hades:
                        fightWithHades(theKnight, eventIndex);
                        break;
                    case ScarletHakama:
                        hadScarletHakama = true;
                        break;
                    case LockedDoor:
                        if (isLancelot || isDragonKnight || (theKnight.level > (eventIndex % 10))) getOverChallenge = 1;
                        else getOverChallenge = 2;
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
                nPetal = (nPetal == 0) ? nPetal : nPetal - 1;
                if (getOverChallenge == 2) {
                    getOverChallenge = 0;
                    break;
                }    
            }
            if (!beatUltimecia) {
                int tempLevel = theKnight.level;
                theKnight.level = (theKnight.level == 10) ? 10 : theKnight.level + 1;
                maxHP = ((maxHP + 100) > 999) && (theKnight.level != tempLevel) ? 999 : maxHP + 100;
            }
        }
        if (beatUltimecia) {
            bFlag = 1;
            break;
        }
        else if ( !isArthur && (nPetal == 0) && !beatUltimecia) {
            bFlag = 0;
            break;
        }
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
