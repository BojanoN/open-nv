#pragma once
#include <cstdint>

typedef uint32_t formid;

namespace ESM {

enum class GroupType : uint32_t {
    TopLevel,
    WorldChildren,
    InteriorCellBlock,
    InteriorCellSubBlock,
    ExteriorCellBlock,
    CellChildren,
    TopicChildren,
    CellPersistentChildren,
    CellTemporaryChildren,
    CellVisibleDistantChildren,
};

enum ESMType : uint32_t {
    NO_ENTRY = 0,

    ACHR = 0x52484341,
    ACRE = 0x45524341,
    ACTI = 0x49544341,
    ADDN = 0x4e444441,
    ALCH = 0x48434c41,
    ALOC = 0x434f4c41,
    AMEF = 0x46454d41,
    AMMO = 0x4f4d4d41,
    ANIO = 0x4f494e41,
    ARMO = 0x4f4d5241,
    ARMA = 0x414d5241,
    ASPC = 0x43505341,
    AVIF = 0x46495641,
    BOOK = 0x4b4f4f42,
    BPTD = 0x44545042,
    CAMS = 0x534d4143,
    CCRD = 0x44524343,
    CDCK = 0x4b434443,
    CELL = 0x4c4c4543,
    CHAL = 0x4c414843,
    CHIP = 0x50494843,
    CLAS = 0x53414c43,
    CLMT = 0x544d4c43,
    CMNY = 0x594e4d43,
    COBJ = 0x4a424f43,
    CONT = 0x544e4f43,
    CPTH = 0x48545043,
    CREA = 0x41455243,
    CSNO = 0x4f4e5343,
    CSTY = 0x59545343,
    DEBR = 0x52424544,
    DEHY = 0x59484544,
    DIAL = 0x4c414944,
    DOBJ = 0x4a424f44,
    DOOR = 0x524f4f44,
    ECZN = 0x4e5a4345,
    EFSH = 0x48534645,
    ENCH = 0x48434e45,
    EXPL = 0x4c505845,
    EYES = 0x53455945,
    FACT = 0x54434146,
    FLST = 0x54534c46,
    FURN = 0x4e525546,
    GLOB = 0x424f4c47,
    GMST = 0x54534d47,
    GRAS = 0x53415247,
    HAIR = 0x52494148,
    HDPT = 0x54504448,
    HUNG = 0x474e5548,
    IDLE = 0x454c4449,
    IDLM = 0x4d4c4449,
    IMGS = 0x53474d49,
    IMAD = 0x44414d49,
    IMOD = 0x444f4d49,
    INFO = 0x4f464e49,
    INGR = 0x52474e49,
    IPCT = 0x54435049,
    IPDS = 0x53445049,
    KEYM = 0x4d59454b,
    LAND = 0x444e414c,
    LGTM = 0x4d54474c,
    LIGH = 0x4847494c,
    LSCR = 0x5243534c,
    LSCT = 0x5443534c,
    LTEX = 0x5845544c,
    LVLC = 0x434c564c,
    LVLI = 0x494c564c,
    LVLN = 0x4e4c564c,
    MESG = 0x4753454d,
    MGEF = 0x4645474d,
    MICN = 0x4e43494d,
    MISC = 0x4353494d,
    MSET = 0x5445534d,
    MSTT = 0x5454534d,
    MUSC = 0x4353554d,
    NAVI = 0x4956414e,
    NAVM = 0x4d56414e,
    NOTE = 0x45544f4e,
    NPC_ = 0x5f43504e,
    PACK = 0x4b434150,
    PERK = 0x4b524550,
    PGRE = 0x45524750,
    PMIS = 0x53494d50,
    PROJ = 0x4a4f5250,
    PWAT = 0x54415750,
    QUST = 0x54535551,
    RACE = 0x45434152,
    RADS = 0x53444152,
    RCCT = 0x54434352,
    RCPE = 0x45504352,
    REFR = 0x52464552,
    REGN = 0x4e474552,
    REPU = 0x55504552,
    RGDL = 0x4c444752,
    SCOL = 0x4c4f4353,
    SCPT = 0x54504353,
    SLPD = 0x44504c53,
    SOUN = 0x4e554f53,
    SPEL = 0x4c455053,
    STAT = 0x54415453,
    TACT = 0x54434154,
    TERM = 0x4d524554,
    TES4 = 0x34534554,
    TREE = 0x45455254,
    TXST = 0x54535854,
    VTYP = 0x50595456,
    WATR = 0x52544157,
    WEAP = 0x50414557,
    WRLD = 0x444c5257,
    WTHR = 0x52485457,
    //subrecs
    EDID = 0x44494445,
    DATA = 0x41544144,
    MODL = 0x4c444f4d,
    MOD2 = 0x32444f4d,
    MOD3 = 0x33444f4d,
    MOD4 = 0x34444f4d,
    MODB = 0x42444f4d,
    MODT = 0x54444f4d,
    MO2T = 0x54324f4d,
    MO3T = 0x54334f4d,
    MO4T = 0x54344f4d,
    MODS = 0x53444f4d,
    MO2S = 0x53324f4d,
    MO3S = 0x53334f4d,
    MO4S = 0x53344f4d,
    MODD = 0x44444f4d,
    MOSD = 0x44534f4d,

    OBND = 0x444e424f,
    TX00 = 0x30305854,
    TX01 = 0x31305854,
    TX02 = 0x32305854,
    TX03 = 0x33305854,
    TX04 = 0x34305854,
    TX05 = 0x35305854,
    DODT = 0x54444f44,
    DNAM = 0x4d414e44,

    MICO = 0x4f43494d,

    FULL = 0x4c4c5546,
    DESC = 0x43534544,
    ICON = 0x4e4f4349,
    ATTR = 0x52545441,

    SCHR = 0x52484353,
    SCDS = 0x53444353,
    SCTX = 0x58544353,
    SCRO = 0x4f524353,
    SCRV = 0x56524353,
    SLSD = 0x44534c53,
    SCVR = 0x52564353,

    RNAM = 0x4d414e52,
    MNAM = 0x4d414e4d,
    FNAM = 0x4d414e46,
    INAM = 0x4d414e49,
    XNAM = 0x4d414e58,
    CNAM = 0x4d414e43,
    WMI1 = 0x31494d57,
    HNAM = 0x4d414e48,
    ONAM = 0x4d414e4f,
    YNAM = 0x4d414e59,
    VTCK = 0x4b435456,
    NAM2 = 0x324d414e,
    UNAM = 0x4d414e55,
    NAM0 = 0x304d414e,
    INDX = 0x58444e49,
    NAM1 = 0x314d414e,
    ENAM = 0x4d414e45,
    FGGS = 0x53474746,
    FGTS = 0x53544746,
    FGGA = 0x41474746,
    FLTV = 0x56544c46,
    ENIT = 0x54494e45,
    EFID = 0x44494645,
    EFIT = 0x54494645,
    CTDA = 0x41445443,

    SCRI = 0x49524353,
    SNAM = 0x4d414e53,
    VNAM = 0x4d414e56,
    WNAM = 0x4d414e57,
    XATO = 0x4f544158,

    DEST = 0x54534544,
    DSTD = 0x44545344,
    DMDL = 0x4c444d44,
    DMDT = 0x54444d44,
    DSTF = 0x46545344,

    PNAM = 0x4d414e50,
    ITXT = 0x54585449,
    ANAM = 0x4d414e41,
    TNAM = 0x4d414e54,
    SCDA = 0x41444353,
    SPIT = 0x54495053,
    SNDD = 0x44444e53,
    SNDX = 0x58444e53,
    GNAM = 0x4d414e47,
    RDAT = 0x54414452,

    EITM = 0x4d544945,
    BMDT = 0x54444d42,
    ICO2 = 0x324f4349,
    MIC2 = 0x3243494d,
    BMCT = 0x54434d42,
    REPL = 0x4c504552,
    BIPL = 0x4c504942,
    ETYP = 0x50595445,
    ZNAM = 0x4d414e5a,
    BNAM = 0x4d414e42,
    CNTO = 0x4f544e43,
    COED = 0x44454f43,
    QNAM = 0x4d414e51,
    BRUS = 0x53555242,
    GRUP = 0x50555247,
    RCIL = 0x4c494352,
    DAT2 = 0x32544144,

    FMAM = 0x4d414d46,
    EAMT = 0x544d4145,
    EFSD = 0x44534645,
    MWD1 = 0x3144574d,
    MWD2 = 0x3244574d,
    MWD3 = 0x3344574d,
    MWD4 = 0x3444574d,
    MWD5 = 0x3544574d,
    MWD6 = 0x3644574d,
    MWD7 = 0x3744574d,
    NNAM = 0x4d414e4e,
    WNM1 = 0x314d4e57,
    WNM2 = 0x324d4e57,
    WNM3 = 0x334d4e57,
    WNM4 = 0x344d4e57,
    WNM5 = 0x354d4e57,
    WNM6 = 0x364d4e57,
    WNM7 = 0x374d4e57,
    NAM7 = 0x374d414e,
    NAM6 = 0x364d414e,
    NAM9 = 0x394d414e,
    NAM8 = 0x384d414e,

    WMS1 = 0x31534d57,
    WMS2 = 0x32534d57,
    CRDT = 0x54445243,
    VATS = 0x53544156,

    WMI2 = 0x32494d57,
    WMI3 = 0x33494d57,
    VANM = 0x4d4e4156,

    ACBS = 0x53424341,
    TPLT = 0x544c5054,
    SPLO = 0x4f4c5053,

    AIDT = 0x54444941,
    PKID = 0x44494b50,
    HCLR = 0x524c4348,
    LNAM = 0x4d414e4c,
    NAM4 = 0x344d414e,
    NAM5 = 0x354d414e,

    NIFZ = 0x5a46494e,
    NIFT = 0x5446494e,
    KFFZ = 0x5a46464b,
    CSCR = 0x52435343,
    CSDR = 0x52445343,
    CSDI = 0x49445343,
    CSDC = 0x43445343,
    CSDT = 0x54445343,

    LVLD = 0x444c564c,
    LVLF = 0x464c564c,
    LVLO = 0x4f4c564c,
    IDLF = 0x464c4449,
    IDLC = 0x434c4449,
    IDLT = 0x544c4449,
    IDLA = 0x414c4449,
    LVLG = 0x474c564c,

    IAD0 = 0x44414900,
    IAD1 = 0x44414901,
    IAD2 = 0x44414902,
    IAD3 = 0x44414903,
    IAD4 = 0x44414904,
    IAD5 = 0x44414905,

    WLST = 0x54534c57,

    RDOT = 0x544f4452,
    RDMP = 0x504d4452,
    RDGS = 0x53474452,
    RDMD = 0x444d4452,
    RDMO = 0x4f4d4452,
    RDSI = 0x49534452,
    RDSD = 0x44534452,
    RDWT = 0x54574452,
    RDID = 0x44494452,
    RDSB = 0x42534452,
    RCLR = 0x524c4352,
    RPLI = 0x494c5052,
    RPLD = 0x444c5052,

    XEZN = 0x4e5a4558,
    NAM3 = 0x334d414e,
    IMPF = 0x46504d49,
    OFST = 0x5453464f,
    IMPS = 0x53504d49,
    QSTI = 0x49545351,
    QSTR = 0x52545351,
    INFC = 0x43464e49,
    INFX = 0x58464e49,
    TDUM = 0x4d554454,

    LTMP = 0x504d544c,
    XCLW = 0x574c4358,
    XCLR = 0x524c4358,
    XCIM = 0x4d494358,
    XCET = 0x54454358,
    XCCM = 0x4d434358,
    XCWT = 0x54574358,
    XOWN = 0x4e574f58,
    XRNK = 0x4b4e5258,
    XCLC = 0x434c4358,
    XCLL = 0x4c4c4358,
    XCAS = 0x53414358,
    XCMT = 0x544d4358,
    XCMO = 0x4f4d4358,

};

};
