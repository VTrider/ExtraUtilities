#pragma once

// Credit to Janne for this file -VT

// todo: align structs
// gameobject: in progress
// recycler: done?

typedef unsigned char   undefined;

typedef unsigned int    ImageBaseOffset32;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef long long    longlong;
typedef char    sbyte;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;

typedef struct Net Net, * PNet;
typedef struct PING_PACKET PING_PACKET, * PPING_PACKET;
typedef struct PINGREPLY_PACKET PINGREPLY_PACKET, * PPINGREPLY_PACKET;

typedef struct ScorePlayer ScorePlayer, * PScorePlayer;

typedef struct GameObject GameObject, * PGameObject;

typedef struct OrdnanceClass OrdnanceClass, * POrdnanceClass;

typedef struct AiCmdInfo AiCmdInfo, * PAiCmdInfo;

typedef struct Recycler Recycler, * PRecycler;

typedef struct GameObjectClass GameObjectClass, * PGameObjectClass;

typedef struct tagENTITY tagENTITY, * PtagENTITY;

typedef struct _OBJ76 _OBJ76, * P_OBJ76;

typedef struct AiProcess AiProcess, * PAiProcess;

typedef struct VECTOR_3D VECTOR_3D, * PVECTOR_3D;
typedef struct VECTOR_2D VECTOR_2D, * PVECTOR_2D;

typedef struct EULER EULER, * PEULER;

typedef struct Team Team, * PTeam;

typedef struct Scanner Scanner, * PScanner;

typedef struct Jammer Jammer, * PJammer;

typedef struct Carrier Carrier, * PCarrier;

typedef struct ModeList ModeList, * PModeList;

typedef struct ExplosionClass ExplosionClass, * PExplosionClass;

typedef struct NetPlayer NetPlayer, * PNetPlayer;

typedef struct SpawnPoint SpawnPoint, * PSpawnPoint;

typedef struct ModeButton ModeButton, * PModeButton;

typedef struct CmdButton CmdButton, * PCmdButton;

typedef struct BuildItem BuildItem, * PBuildItem;

typedef union ModeButton_u_0 ModeButton_u_0, * PModeButton_u_0;

typedef struct tagPOINT tagPOINT, * PtagPOINT;

typedef struct tagRECT tagRECT, * PtagRECT;

typedef enum AiCommand {
	CMD_ATTACK = -2147352568,
	CMD_BUILD = -2147352568,
	CMD_DEFEND = -2147352568,
	CMD_DROPOFF = -2147352568,
	CMD_FOLLOW = -2147352568,
	CMD_FORMATION = -2147352568,
	CMD_GET_BOMB = -2147352568,
	CMD_GET_CAMERA = -2147352568,
	CMD_GET_IN = -2147352568,
	CMD_GET_RELOAD = -2147352568,
	CMD_GET_REPAIR = -2147352568,
	CMD_GET_WEAPON = -2147352568,
	CMD_GO = -2147352568,
	CMD_GO_TO_GEYSER = -2147352568,
	CMD_HUNT = -2147352568,
	CMD_LAY_MINES = -2147352568,
	CMD_NONE = -2147352568,
	CMD_NO_DROPOFF = -2147352568,
	CMD_PATROL = -2147352568,
	CMD_PICKUP = -2147352568,
	CMD_RECYCLE = -2147352568,
	CMD_RESCUE = -2147352568,
	CMD_SCAVENGE = -2147352568,
	CMD_SELECT = -2147352568,
	CMD_SEND = -2147352568,
	CMD_STAGE = -2147352568,
	CMD_STOP = -2147352568,
	NUM_CMD = -2147352568
} AiCommand;

typedef struct AiPath AiPath, * PAiPath;

typedef enum OBJECT_CLASS_T {
	CLASS_ID_BRIDGE = -2147352568,
	CLASS_ID_CANNON_HARDPOINT = -2147352568,
	CLASS_ID_CHUNK = -2147352568,
	CLASS_ID_COCKPIT_GEOMETRY = -2147352568,
	CLASS_ID_COM = -2147352568,
	CLASS_ID_DUST_EMITTER = -2147352568,
	CLASS_ID_EXPLOSION = -2147352568,
	CLASS_ID_EYEPOINT = -2147352568,
	CLASS_ID_FIN_GEOMETRY = -2147352568,
	CLASS_ID_FLAME_EMITTER = -2147352568,
	CLASS_ID_FLOOR = -2147352568,
	CLASS_ID_HEADLIGHT_MASK = -2147352568,
	CLASS_ID_HELICOPTER = -2147352568,
	CLASS_ID_MORTAR_HARDPOINT = -2147352568,
	CLASS_ID_NACELLE_GEOMETRY = -2147352568,
	CLASS_ID_NONCOLLIDABLE = -2147352568,
	CLASS_ID_NONE = -2147352568,
	CLASS_ID_ORDNANCE = -2147352568,
	CLASS_ID_ORDNANCE_GEOMETRY = -2147352568,
	CLASS_ID_PARKING_LOT = -2147352568,
	CLASS_ID_PERSON = -2147352568,
	CLASS_ID_POWERUP = -2147352568,
	CLASS_ID_ROCKET_HARDPOINT = -2147352568,
	CLASS_ID_ROTOR_GEOMETRY = -2147352568,
	CLASS_ID_SCRAP = -2147352568,
	CLASS_ID_SCROUNGE = -2147352568,
	CLASS_ID_SIGN = -2147352568,
	CLASS_ID_SMOKE_EMITTER = -2147352568,
	CLASS_ID_SORT_OBJECT = -2147352568,
	CLASS_ID_SPECIAL_HARDPOINT = -2147352568,
	CLASS_ID_SPINNER = -2147352568,
	CLASS_ID_STRUCTURE1 = -2147352568,
	CLASS_ID_STRUCTURE2 = -2147352568,
	CLASS_ID_STRUCTURE_GEOMETRY = -2147352568,
	CLASS_ID_TURRET_GEOMETRY = -2147352568,
	CLASS_ID_VEHICLE = -2147352568,
	CLASS_ID_VEHICLE_GEOMETRY = -2147352568,
	CLASS_ID_WEAPON = -2147352568,
	CLASS_ID_WEAPON_GEOMETRY = -2147352568,
	CLASS_ID_WEAPON_HARDPOINT = -2147352568
} OBJECT_CLASS_T;

typedef struct WeaponClass WeaponClass, * PWeaponClass;

typedef struct BBOX BBOX, * PBBOX;

typedef struct SPHERE SPHERE, * PSPHERE;

typedef enum CLSN_TYPE {
	CLSN_TYPE_CAR = -2147352568,
	CLSN_TYPE_CHILDREN = -2147352568,
	CLSN_TYPE_NONCOLLIDE = -2147352568,
	CLSN_TYPE_SPHERE = -2147352568
} CLSN_TYPE;

typedef struct BSP_CONTAINER BSP_CONTAINER, * PBSP_CONTAINER;

typedef union _OBJHANDLE _OBJHANDLE, * P_OBJHANDLE;

typedef struct MAT_3D MAT_3D, * PMAT_3D;

typedef struct _GEOMETRY _GEOMETRY, * P_GEOMETRY;

typedef struct _REPS _REPS, * P_REPS;

typedef struct TEX_INFO TEX_INFO, * PTEX_INFO;

typedef struct dibs_stack dibs_stack, * Pdibs_stack;

typedef struct AiMission AiMission, * PAiMission;

typedef struct Weapon Weapon, * PWeapon;

typedef struct BSP_POLYGON BSP_POLYGON, * PBSP_POLYGON;

typedef union _OBJHANDLE_bitfield_0 _OBJHANDLE_bitfield_0, * P_OBJHANDLE_bitfield_0;

typedef struct _CACHE_INFO _CACHE_INFO, * P_CACHE_INFO;

typedef struct FACE FACE, * PFACE;

typedef struct _REP_INFO _REP_INFO, * P_REP_INFO;

typedef struct textab textab, * Ptextab;

typedef struct TEXTURE TEXTURE, * PTEXTURE;

typedef enum BSP_GEOM_TYPE {
	BSP_I76_OBJ = -2147352568,
	BSP_SINGLE_POLYGON = -2147352568,
	BSP_SPRITE = -2147352568
} BSP_GEOM_TYPE;

typedef struct PLANE PLANE, * PPLANE;

typedef struct POINT_3D POINT_3D, * PPOINT_3D;

typedef struct COLOR COLOR, * PCOLOR;

typedef struct FACE_NODE FACE_NODE, * PFACE_NODE;

typedef struct TEXEL TEXEL, * PTEXEL;

typedef union LUMA LUMA, * PLUMA;

typedef union TEXEL_u_8 TEXEL_u_8, * PTEXEL_u_8;

typedef struct unnamed_0045 unnamed_0045, * Punnamed_0045;

struct AiCmdInfo {
	int priority;
	enum AiCommand what;
	int who;
	struct AiPath* where;
	float when;
	long param;
};

struct Scanner {
	float range;
	float period;
	bool active;
	float sweep;
	struct GameObject* owner;
};

struct ExplosionClass {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	long sig;
	char* label;
	__int64 cfg;
	char odf[16];
	struct ExplosionClass* proto;
	struct _OBJ76* freeObj;
	long frameIndex;
	long frameCount;
	float frameRate;
	float explRadius;
	char explSound[16];
	float damageRadius;
	float damageValue;
	ushort damageTypes;
	undefined field_0x5a;
	undefined field_0x5b;
	float omegaShake;
	float deltaShake;
};

struct _GEOMETRY {
	struct _CACHE_INFO* cache_info;
	long vertex_count;
	long normal_count;
	struct VECTOR_3D* vertex_table;
	struct VECTOR_3D* normal_table;
	struct FACE* faces;
	uchar* Data;
};

struct AiProcess {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	struct AiMission* fMission;
	struct GameObject* fOwner;
	int exited;
};

struct _CACHE_INFO {
	__int64 id;
	char* data;
	int refCount;
	int size;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
};

struct MAT_3D {
	float right_x;
	float right_y;
	float right_z;
	float up_x;
	float up_y;
	float up_z;
	float front_x;
	float front_y;
	float front_z;
	undefined field_0x24;
	undefined field_0x25;
	undefined field_0x26;
	undefined field_0x27;
	double posit_x;
	double posit_y;
	double posit_z;
};

struct Weapon {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	struct WeaponClass* weaponClass;
	struct OrdnanceClass* ordnanceClass;
	struct _OBJ76* obj;
	struct _OBJ76* hard;
	struct _OBJ76* owner;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	struct MAT_3D M;
	struct MAT_3D I;
	int wpnReticle;
	float fLastTriggered;
};

struct VECTOR_3D {
	float x;
	float y;
	float z;
};

struct VECTOR_2D {
	float x;
	float z;
};

struct unnamed_0045 {
	uchar intensity;
	uchar fogginess;
};

union TEXEL_u_8 {
	float luma;
	struct unnamed_0045 l;
};

struct TEXEL {
	float u;
	float v;
	union TEXEL_u_8 _u_8;
};

struct POINT_3D {
	struct VECTOR_3D Vector;
	struct TEXEL Texel;
};

union _OBJHANDLE_bitfield_0 {
	uint index : 0xb;
	uint seqNo : 0x15;
};

union _OBJHANDLE {
	union _OBJHANDLE_bitfield_0 _bitfield_0;
	int handle;
};

struct WeaponClass {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	struct WeaponClass* proto;
	ulong sig;
	char* label;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	__int64 cfg;
	char odf[16];
	struct OrdnanceClass* ordnanceClass;
	int wpnNameIndex;
	char wpnName[32];
	char fireSound[16];
	int wpnReticle;
	int wpnLadder;
	long wpnPriority;
	enum OBJECT_CLASS_T wpnCategory;
};



struct AiPath {
	undefined unknown[8];
	VECTOR_2D pos;
};

struct EULER {
	float mass;
	float mass_inv;
	float v_mag;
	float v_mag_inv;
	float I;
	float k_i;
	struct VECTOR_3D v;
	struct VECTOR_3D omega;
	struct VECTOR_3D Accel;
	struct VECTOR_3D Alpha;
};

struct ModeList {
	int modeList[11];
	ulong enabledMask;
	int activeSlot;
};

struct GameObject {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	undefined field_0x20;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	undefined field_0x24;
	undefined field_0x25;
	undefined field_0x26;
	undefined field_0x27;
	undefined field_0x28;
	undefined field_0x29;
	undefined field_0x2a;
	undefined field_0x2b;
	undefined field_0x2c;
	undefined field_0x2d;
	undefined field_0x2e;
	undefined field_0x2f;
	undefined field_0x30;
	undefined field_0x31;
	undefined field_0x32;
	undefined field_0x33;
	undefined field_0x34;
	undefined field_0x35;
	undefined field_0x36;
	undefined field_0x37;
	undefined field_0x38;
	undefined field_0x39;
	undefined field_0x3a;
	undefined field_0x3b;
	undefined field_0x3c;
	undefined field_0x3d;
	undefined field_0x3e;
	undefined field_0x3f;
	undefined field_0x40;
	undefined field_0x41;
	undefined field_0x42;
	undefined field_0x43;
	undefined field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	undefined field_0x48;
	undefined field_0x49;
	undefined field_0x4a;
	undefined field_0x4b;
	undefined field_0x4c;
	undefined field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;
	undefined field_0x50;
	undefined field_0x51;
	undefined field_0x52;
	undefined field_0x53;
	undefined field_0x54;
	undefined field_0x55;
	undefined field_0x56;
	undefined field_0x57;
	undefined field_0x58;
	undefined field_0x59;
	undefined field_0x5a;
	undefined field_0x5b;
	undefined field_0x5c;
	undefined field_0x5d;
	undefined field_0x5e;
	undefined field_0x5f;
	undefined field_0x60;
	undefined field_0x61;
	undefined field_0x62;
	undefined field_0x63;
	undefined field_0x64;
	undefined field_0x65;
	undefined field_0x66;
	undefined field_0x67;
	undefined field_0x68;
	undefined field_0x69;
	undefined field_0x6a;
	undefined field_0x6b;
	undefined field_0x6c;
	undefined field_0x6d;
	undefined field_0x6e;
	undefined field_0x6f;
	undefined field_0x70;
	undefined field_0x71;
	undefined field_0x72;
	undefined field_0x73;
	undefined field_0x74;
	undefined field_0x75;
	undefined field_0x76;
	undefined field_0x77;
	undefined field_0x78;
	undefined field_0x79;
	undefined field_0x7a;
	undefined field_0x7b;
	undefined field_0x7c;
	undefined field_0x7d;
	undefined field_0x7e;
	undefined field_0x7f;
	undefined field_0x80;
	undefined field_0x81;
	undefined field_0x82;
	undefined field_0x83;
	undefined field_0x84;
	undefined field_0x85;
	undefined field_0x86;
	undefined field_0x87;
	undefined field_0x88;
	undefined field_0x89;
	undefined field_0x8a;
	undefined field_0x8b;
	undefined field_0x8c;
	undefined field_0x8d;
	undefined field_0x8e;
	undefined field_0x8f;
	undefined field_0x90;
	undefined field_0x91;
	undefined field_0x92;
	undefined field_0x93;
	undefined field_0x94;
	undefined field_0x95;
	undefined field_0x96;
	undefined field_0x97;
	undefined field_0x98;
	undefined field_0x99;
	undefined field_0x9a;
	undefined field_0x9b;
	undefined field_0x9c;
	undefined field_0x9d;
	undefined field_0x9e;
	undefined field_0x9f;
	struct OrdnanceClass* hitOrdClass;
	bool isCargo;
	undefined field_0xa5;
	undefined field_0xa6;
	undefined field_0xa7;
	struct GameObject* tug;
	struct AiCmdInfo curCmd;
	struct AiCmdInfo nextCmd;
	float illumination;
	struct GameObjectClass* curPilot;
	struct tagENTITY* ent;
	struct _OBJ76* obj;
	struct GameObjectClass* objClass;
	struct AiProcess* aiProcess;
	char* label;
	int independence;
	struct VECTOR_3D pos;
	struct EULER euler;
	int seqNo;
	int indexNo;
	float collisionRadius;
	ulong createTime;
	long liveColor;
	long deadColor;
	int teamNumber;
	int teamSlot;
	struct Team* teamList;
	int perceivedTeam;
	int nameIndex;
	char* name;
	bool isObjective;
	bool isSelected;
	undefined field_0x182;
	undefined field_0x183;
	ulong isVisible;
	ulong seen;
	ulong locked;
	struct Scanner* scanner;
	struct Jammer* jammer;
	struct Carrier* carrier;
	struct ModeList modeList;
	float playerShot;
	float playerCollide;
	float friendShot;
	float friendCollide;
	float enemyShot;
	float enemyCollide;
	float groundCollide;
	int who_shot_JR;
	int who_i_shot;
	float when_i_shot;
	float healthRatio;
	float curHealth;
	float maxHealth;
	float ammoRatio;
	long curAmmo;
	long maxAmmo;
	long weaponMask;
	int targetHandle;
	struct _OBJ76* hitch;
	int ownerHandle;
};

struct AiMission {
};

struct COLOR {
	uchar index;
	uchar red;
	uchar green;
	uchar blue;
};

struct dibs_stack {
	char* owner;
	struct dibs_stack* prev;
	struct dibs_stack* next;
};

struct Team {
	struct GameObject* teamSlot[90];
	long curScrap;
	long maxScrap;
	long curPilot;
	long maxPilot;
	ulong dwAllies;
	int teamNumber;
};

struct SPHERE {
	struct VECTOR_3D origin;
	float radius;
};

struct BBOX {
	struct VECTOR_3D min;
	struct VECTOR_3D max;
};

struct tagENTITY {
	struct _OBJ76* obj76;
	ushort seqNo;
	uchar lodFlags;
	uchar animFlags;
	struct SPHERE bSphere;
	struct VECTOR_3D worldOrigin;
	enum CLSN_TYPE cType;
	void* clsnDetails;
	float lodRanges[5];
	int lodRangesSet;
	struct BBOX bBox;
	long Bsp_Geom_LOD;
	long Bsp_Vertex_Pool_Size;
	long Bsp_LRU_Stamp;
	struct BSP_CONTAINER* Bsp_Container;
	struct VECTOR_3D shadowPoly[4];
	struct tagENTITY* mergeNext;
	struct tagENTITY* mergePrev;
	int mergeCount;
	void* mergePoly;
	struct VECTOR_3D min;
	struct VECTOR_3D max;
};

struct Carrier {
	struct GameObject* owner;
	struct _OBJ76* hardpoint[5];
	struct Weapon* weapon[5];
	ulong existant;
	ulong selected;
	ulong enabled;
	int special;
	float fWeaponTriggerTillTime;
};

struct TEX_INFO {
	char group[14];
	undefined field_0xe;
	undefined field_0xf;
	struct textab* textab;
	long selectNum;
	__int64 selectID;
	struct TEXTURE* selectTex;
	undefined field_0x24;
	undefined field_0x25;
	undefined field_0x26;
	undefined field_0x27;
};

struct OrdnanceClass {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	struct OrdnanceClass* proto;
	ulong sig;
	char* label;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	__int64 cfg;
	char odf[16];
	struct _OBJ76* ord;
	struct tagENTITY* ent;
	struct _OBJ76* freeOrd;
	struct ExplosionClass* xplGround;
	struct ExplosionClass* xplVehicle;
	struct ExplosionClass* xplBuilding;
	long ammoCost;
	float lifeSpan;
	float shotSpeed;
	float damageValue;
	ushort damageTypes;
	bool notifyRemote;
	char shotSound[16];
};

struct _REPS {
	int repCount;
	int repSlots;
	long repNum;
	struct _REP_INFO* reps;
};

struct _OBJ76 {
	union _OBJHANDLE objHandle;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	__int64 id;
	ushort gravestone;
	ushort counter;
	ulong flags;
	long render_type_override;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	struct MAT_3D transform;
	long NavGroup;
	struct _GEOMETRY* geom;
	struct _REPS reps;
	struct TEX_INFO tex_info;
	struct _OBJ76* parent;
	struct _OBJ76* sibling;
	struct _OBJ76* child;
	enum OBJECT_CLASS_T class_id;
	void* class_ptr;
	void* gameObj;
	struct dibs_stack* createdby;
	struct dibs_stack* dibs;
	struct dibs_stack* removedby;
	void* clsnInfo;
	struct SPHERE bSphere;
	struct BBOX bBox;
};

union LUMA {
	float f;
	long index;
};

struct FACE_NODE {
	long vector_index;
	union LUMA luminance;
	float u;
	float v;
};

struct PLANE {
	struct VECTOR_3D surf_normal;
	float dist;
};

struct BSP_POLYGON {
	enum BSP_GEOM_TYPE Bsp_Type;
	long Count;
	struct _OBJ76* Obj76;
	char Texture_name[16];
	long Color_Index;
	long Bitmap_Operation;
	struct VECTOR_3D Test;
	struct PLANE Plane;
	struct BSP_POLYGON* Front;
	struct BSP_POLYGON* Back;
	struct POINT_3D* Vertex;
};

struct TEXTURE {
	short width;
	short type;
	long height;
	uchar* bytes;
};

struct BSP_CONTAINER {
	struct BSP_CONTAINER* Next;
	struct BSP_POLYGON* Bsp_Geom;
	char* Data;
};

struct Jammer {
	float maxSpeed;
	struct GameObject* owner;
};

struct textab {
	char group[16];
	long usage_count;
	float FramesPerSec;
	ulong flags;
	long num_dims;
	long size_dims[3];
	char* texture_array;
	int next;
};


typedef enum VEHICLE_STATE {
	DEPLOYED = 2,
	DEPLOYING = 1,
	UNDEPLOYED = 0,
	UNDEPLOYING = 3
} VEHICLE_STATE;


struct GameObjectClass {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	ulong sig;
	char* label;
	enum OBJECT_CLASS_T class_id;
	struct GameObjectClass* proto;
	__int64 base;
	__int64 cfg;
	char odf[16];
	long scrapCost;
	long scrapValue;
	long pilotCost;
	float buildTime;
	long maxHealth;
	long maxAmmo;
	int unitNameIndex;
	char unitName[32];
	char aiName[32];
	char aiName2[32];
	long weaponMask;
	float collisionRadius;
	char nation;
	undefined field_0xd5;
	undefined field_0xd6;
	undefined field_0xd7;
	struct GameObjectClass* pilotClass;
	struct GameObjectClass* userClass;
	float imageSignature;
	float radarSignature;
	float heatSignature;
	char weaponHard[5][4];
	struct WeaponClass* weaponClass[5];
	struct BBOX bBox;
	struct SPHERE bSphere;
	float boundingWidth;
	float boundingHeight;
	int categoryMin;
	int categoryMax;
	char* infoText;
};

struct _REP_INFO {
	long repNum;
	struct _GEOMETRY* geom;
	__int64 id;
};

struct FACE {
	long index;
	long vertex_count;
	struct COLOR color;
	struct PLANE plane;
	uchar ShadeType;
	uchar TextureType;
	uchar XluscentType;
	uchar padding;
	char texture_name[16];
	struct FACE* front_facet;
	struct VECTOR_3D Test_Point;
	struct FACE_NODE* wire_frame;
};

struct Craft {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	undefined field_0x20;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	undefined field_0x24;
	undefined field_0x25;
	undefined field_0x26;
	undefined field_0x27;
	undefined field_0x28;
	undefined field_0x29;
	undefined field_0x2a;
	undefined field_0x2b;
	undefined field_0x2c;
	undefined field_0x2d;
	undefined field_0x2e;
	undefined field_0x2f;
	undefined field_0x30;
	undefined field_0x31;
	undefined field_0x32;
	undefined field_0x33;
	undefined field_0x34;
	undefined field_0x35;
	undefined field_0x36;
	undefined field_0x37;
	undefined field_0x38;
	undefined field_0x39;
	undefined field_0x3a;
	undefined field_0x3b;
	undefined field_0x3c;
	undefined field_0x3d;
	undefined field_0x3e;
	undefined field_0x3f;
	undefined field_0x40;
	undefined field_0x41;
	undefined field_0x42;
	undefined field_0x43;
	undefined field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	undefined field_0x48;
	undefined field_0x49;
	undefined field_0x4a;
	undefined field_0x4b;
	undefined field_0x4c;
	undefined field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;
	undefined field_0x50;
	undefined field_0x51;
	undefined field_0x52;
	undefined field_0x53;
	undefined field_0x54;
	undefined field_0x55;
	undefined field_0x56;
	undefined field_0x57;
	undefined field_0x58;
	undefined field_0x59;
	undefined field_0x5a;
	undefined field_0x5b;
	undefined field_0x5c;
	undefined field_0x5d;
	undefined field_0x5e;
	undefined field_0x5f;
	undefined field_0x60;
	undefined field_0x61;
	undefined field_0x62;
	undefined field_0x63;
	undefined field_0x64;
	undefined field_0x65;
	undefined field_0x66;
	undefined field_0x67;
	undefined field_0x68;
	undefined field_0x69;
	undefined field_0x6a;
	undefined field_0x6b;
	undefined field_0x6c;
	undefined field_0x6d;
	undefined field_0x6e;
	undefined field_0x6f;
	undefined field_0x70;
	undefined field_0x71;
	undefined field_0x72;
	undefined field_0x73;
	undefined field_0x74;
	undefined field_0x75;
	undefined field_0x76;
	undefined field_0x77;
	undefined field_0x78;
	undefined field_0x79;
	undefined field_0x7a;
	undefined field_0x7b;
	undefined field_0x7c;
	undefined field_0x7d;
	undefined field_0x7e;
	undefined field_0x7f;
	undefined field_0x80;
	undefined field_0x81;
	undefined field_0x82;
	undefined field_0x83;
	undefined field_0x84;
	undefined field_0x85;
	undefined field_0x86;
	undefined field_0x87;
	undefined field_0x88;
	undefined field_0x89;
	undefined field_0x8a;
	undefined field_0x8b;
	undefined field_0x8c;
	undefined field_0x8d;
	undefined field_0x8e;
	undefined field_0x8f;
	undefined field_0x90;
	undefined field_0x91;
	undefined field_0x92;
	undefined field_0x93;
	undefined field_0x94;
	undefined field_0x95;
	undefined field_0x96;
	undefined field_0x97;
	undefined field_0x98;
	undefined field_0x99;
	undefined field_0x9a;
	undefined field_0x9b;
	undefined field_0x9c;
	undefined field_0x9d;
	undefined field_0x9e;
	undefined field_0x9f;
	undefined field_0xa0;
	undefined field_0xa1;
	undefined field_0xa2;
	undefined field_0xa3;
	undefined field_0xa4;
	undefined field_0xa5;
	undefined field_0xa6;
	undefined field_0xa7;
	undefined field_0xa8;
	undefined field_0xa9;
	undefined field_0xaa;
	undefined field_0xab;
	undefined field_0xac;
	undefined field_0xad;
	undefined field_0xae;
	undefined field_0xaf;
	undefined field_0xb0;
	undefined field_0xb1;
	undefined field_0xb2;
	undefined field_0xb3;
	undefined field_0xb4;
	undefined field_0xb5;
	undefined field_0xb6;
	undefined field_0xb7;
	undefined field_0xb8;
	undefined field_0xb9;
	undefined field_0xba;
	undefined field_0xbb;
	undefined field_0xbc;
	undefined field_0xbd;
	undefined field_0xbe;
	undefined field_0xbf;
	undefined field_0xc0;
	undefined field_0xc1;
	undefined field_0xc2;
	undefined field_0xc3;
	undefined field_0xc4;
	undefined field_0xc5;
	undefined field_0xc6;
	undefined field_0xc7;
	undefined field_0xc8;
	undefined field_0xc9;
	undefined field_0xca;
	undefined field_0xcb;
	undefined field_0xcc;
	undefined field_0xcd;
	undefined field_0xce;
	undefined field_0xcf;
	undefined field_0xd0;
	undefined field_0xd1;
	undefined field_0xd2;
	undefined field_0xd3;
	undefined field_0xd4;
	undefined field_0xd5;
	undefined field_0xd6;
	undefined field_0xd7;
	undefined field_0xd8;
	undefined field_0xd9;
	undefined field_0xda;
	undefined field_0xdb;
	undefined field_0xdc;
	undefined field_0xdd;
	undefined field_0xde;
	undefined field_0xdf;
	undefined field_0xe0;
	undefined field_0xe1;
	undefined field_0xe2;
	undefined field_0xe3;
	undefined field_0xe4;
	undefined field_0xe5;
	undefined field_0xe6;
	undefined field_0xe7;
	undefined field_0xe8;
	undefined field_0xe9;
	undefined field_0xea;
	undefined field_0xeb;
	undefined field_0xec;
	undefined field_0xed;
	undefined field_0xee;
	undefined field_0xef;
	undefined field_0xf0;
	undefined field_0xf1;
	undefined field_0xf2;
	undefined field_0xf3;
	undefined field_0xf4;
	undefined field_0xf5;
	undefined field_0xf6;
	undefined field_0xf7;
	undefined field_0xf8;
	undefined field_0xf9;
	undefined field_0xfa;
	undefined field_0xfb;
	undefined field_0xfc;
	undefined field_0xfd;
	undefined field_0xfe;
	undefined field_0xff;
	undefined field_0x100;
	undefined field_0x101;
	undefined field_0x102;
	undefined field_0x103;
	undefined field_0x104;
	undefined field_0x105;
	undefined field_0x106;
	undefined field_0x107;
	undefined field_0x108;
	undefined field_0x109;
	undefined field_0x10a;
	undefined field_0x10b;
	undefined field_0x10c;
	undefined field_0x10d;
	undefined field_0x10e;
	undefined field_0x10f;
	undefined field_0x110;
	undefined field_0x111;
	undefined field_0x112;
	undefined field_0x113;
	undefined field_0x114;
	undefined field_0x115;
	undefined field_0x116;
	undefined field_0x117;
	undefined field_0x118;
	undefined field_0x119;
	undefined field_0x11a;
	undefined field_0x11b;
	undefined field_0x11c;
	undefined field_0x11d;
	undefined field_0x11e;
	undefined field_0x11f;
	undefined field_0x120;
	undefined field_0x121;
	undefined field_0x122;
	undefined field_0x123;
	undefined field_0x124;
	undefined field_0x125;
	undefined field_0x126;
	undefined field_0x127;
	undefined field_0x128;
	undefined field_0x129;
	undefined field_0x12a;
	undefined field_0x12b;
	undefined field_0x12c;
	undefined field_0x12d;
	undefined field_0x12e;
	undefined field_0x12f;
	undefined field_0x130;
	undefined field_0x131;
	undefined field_0x132;
	undefined field_0x133;
	undefined field_0x134;
	undefined field_0x135;
	undefined field_0x136;
	undefined field_0x137;
	undefined field_0x138;
	undefined field_0x139;
	undefined field_0x13a;
	undefined field_0x13b;
	undefined field_0x13c;
	undefined field_0x13d;
	undefined field_0x13e;
	undefined field_0x13f;
	undefined field_0x140;
	undefined field_0x141;
	undefined field_0x142;
	undefined field_0x143;
	undefined field_0x144;
	undefined field_0x145;
	undefined field_0x146;
	undefined field_0x147;
	undefined field_0x148;
	undefined field_0x149;
	undefined field_0x14a;
	undefined field_0x14b;
	undefined field_0x14c;
	undefined field_0x14d;
	undefined field_0x14e;
	undefined field_0x14f;
	undefined field_0x150;
	undefined field_0x151;
	undefined field_0x152;
	undefined field_0x153;
	undefined field_0x154;
	undefined field_0x155;
	undefined field_0x156;
	undefined field_0x157;
	undefined field_0x158;
	undefined field_0x159;
	undefined field_0x15a;
	undefined field_0x15b;
	undefined field_0x15c;
	undefined field_0x15d;
	undefined field_0x15e;
	undefined field_0x15f;
	undefined field_0x160;
	undefined field_0x161;
	undefined field_0x162;
	undefined field_0x163;
	undefined field_0x164;
	undefined field_0x165;
	undefined field_0x166;
	undefined field_0x167;
	undefined field_0x168;
	undefined field_0x169;
	undefined field_0x16a;
	undefined field_0x16b;
	undefined field_0x16c;
	undefined field_0x16d;
	undefined field_0x16e;
	undefined field_0x16f;
	undefined field_0x170;
	undefined field_0x171;
	undefined field_0x172;
	undefined field_0x173;
	undefined field_0x174;
	undefined field_0x175;
	undefined field_0x176;
	undefined field_0x177;
	undefined field_0x178;
	undefined field_0x179;
	undefined field_0x17a;
	undefined field_0x17b;
	undefined field_0x17c;
	undefined field_0x17d;
	undefined field_0x17e;
	undefined field_0x17f;
	undefined field_0x180;
	undefined field_0x181;
	undefined field_0x182;
	undefined field_0x183;
	undefined field_0x184;
	undefined field_0x185;
	undefined field_0x186;
	undefined field_0x187;
	undefined field_0x188;
	undefined field_0x189;
	undefined field_0x18a;
	undefined field_0x18b;
	undefined field_0x18c;
	undefined field_0x18d;
	undefined field_0x18e;
	undefined field_0x18f;
	undefined field_0x190;
	undefined field_0x191;
	undefined field_0x192;
	undefined field_0x193;
	undefined field_0x194;
	undefined field_0x195;
	undefined field_0x196;
	undefined field_0x197;
	undefined field_0x198;
	undefined field_0x199;
	undefined field_0x19a;
	undefined field_0x19b;
	undefined field_0x19c;
	undefined field_0x19d;
	undefined field_0x19e;
	undefined field_0x19f;
	undefined field_0x1a0;
	undefined field_0x1a1;
	undefined field_0x1a2;
	undefined field_0x1a3;
	undefined field_0x1a4;
	undefined field_0x1a5;
	undefined field_0x1a6;
	undefined field_0x1a7;
	undefined field_0x1a8;
	undefined field_0x1a9;
	undefined field_0x1aa;
	undefined field_0x1ab;
	undefined field_0x1ac;
	undefined field_0x1ad;
	undefined field_0x1ae;
	undefined field_0x1af;
	undefined field_0x1b0;
	undefined field_0x1b1;
	undefined field_0x1b2;
	undefined field_0x1b3;
	undefined field_0x1b4;
	undefined field_0x1b5;
	undefined field_0x1b6;
	undefined field_0x1b7;
	undefined field_0x1b8;
	undefined field_0x1b9;
	undefined field_0x1ba;
	undefined field_0x1bb;
	undefined field_0x1bc;
	undefined field_0x1bd;
	undefined field_0x1be;
	undefined field_0x1bf;
	undefined field_0x1c0;
	undefined field_0x1c1;
	undefined field_0x1c2;
	undefined field_0x1c3;
	undefined field_0x1c4;
	undefined field_0x1c5;
	undefined field_0x1c6;
	undefined field_0x1c7;
	undefined field_0x1c8;
	undefined field_0x1c9;
	undefined field_0x1ca;
	undefined field_0x1cb;
	undefined field_0x1cc;
	undefined field_0x1cd;
	undefined field_0x1ce;
	undefined field_0x1cf;
	undefined field_0x1d0;
	undefined field_0x1d1;
	undefined field_0x1d2;
	undefined field_0x1d3;
	undefined field_0x1d4;
	undefined field_0x1d5;
	undefined field_0x1d6;
	undefined field_0x1d7;
	undefined field_0x1d8;
	undefined field_0x1d9;
	undefined field_0x1da;
	undefined field_0x1db;
	undefined field_0x1dc;
	undefined field_0x1dd;
	undefined field_0x1de;
	undefined field_0x1df;
	undefined field_0x1e0;
	undefined field_0x1e1;
	undefined field_0x1e2;
	undefined field_0x1e3;
	undefined field_0x1e4;
	undefined field_0x1e5;
	undefined field_0x1e6;
	undefined field_0x1e7;
	undefined field_0x1e8;
	undefined field_0x1e9;
	undefined field_0x1ea;
	undefined field_0x1eb;
	undefined field_0x1ec;
	undefined field_0x1ed;
	undefined field_0x1ee;
	undefined field_0x1ef;
	undefined field_0x1f0;
	undefined field_0x1f1;
	undefined field_0x1f2;
	undefined field_0x1f3;
	undefined field_0x1f4;
	undefined field_0x1f5;
	undefined field_0x1f6;
	undefined field_0x1f7;
	undefined field_0x1f8;
	undefined field_0x1f9;
	undefined field_0x1fa;
	undefined field_0x1fb;
	undefined field_0x1fc;
	undefined field_0x1fd;
	undefined field_0x1fe;
	undefined field_0x1ff;
	undefined field_0x200;
	undefined field_0x201;
	undefined field_0x202;
	undefined field_0x203;
	undefined field_0x204;
	undefined field_0x205;
	undefined field_0x206;
	undefined field_0x207;
	undefined field_0x208;
	undefined field_0x209;
	undefined field_0x20a;
	undefined field_0x20b;
	undefined field_0x20c;
	undefined field_0x20d;
	undefined field_0x20e;
	undefined field_0x20f;
	undefined field_0x210;
	undefined field_0x211;
	undefined field_0x212;
	undefined field_0x213;
	undefined field_0x214;
	undefined field_0x215;
	undefined field_0x216;
	undefined field_0x217;
	undefined field_0x218;
	undefined field_0x219;
	undefined field_0x21a;
	undefined field_0x21b;
	undefined field_0x21c;
	undefined field_0x21d;
	undefined field_0x21e;
	undefined field_0x21f;
	enum VEHICLE_STATE state;
	int abandoned;
	struct VEHICLE* vhcl;
	int pathingType;
	struct SmokeEmitter* smokeEmitter;
	float smokeTimer;
	struct _OBJ76* smokeList[8];
	long smokeCount;
	float pitchAng;
	ulong lastCollideTime;
	float fPersonEjectRatio;
};

struct RecyclerProcess {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	enum State curState;
	enum State nextState;
	struct Recycler* craft;
	bool release;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	struct AiPath* where;
	int whoHandle;
	float lastHit;
	struct AiTask* task;
	struct GameObjectClass* classtobuild;
	struct GameObject* target;
	bool isInTransition;
	bool wasInTransition;
};


struct ArmoryProcess {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	enum State curState;
	enum State nextState;
	struct Armory* craft;
	bool release;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	struct AiPath* where;
	int whoHandle;
	struct AiTask* task;
	struct GameObjectClass* classtobuild;
	struct GameObject* target;
	struct VECTOR_3D pos;
	bool isInTransition;
	bool wasInTransition;
};
// ALIGNED
struct Producer {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	undefined field_0x8;
	undefined field_0x9;
	undefined field_0xa;
	undefined field_0xb;
	undefined field_0xc;
	undefined field_0xd;
	undefined field_0xe;
	undefined field_0xf;
	undefined field_0x10;
	undefined field_0x11;
	undefined field_0x12;
	undefined field_0x13;
	undefined field_0x14;
	undefined field_0x15;
	undefined field_0x16;
	undefined field_0x17;
	undefined field_0x18;
	undefined field_0x19;
	undefined field_0x1a;
	undefined field_0x1b;
	undefined field_0x1c;
	undefined field_0x1d;
	undefined field_0x1e;
	undefined field_0x1f;
	undefined field_0x20;
	undefined field_0x21;
	undefined field_0x22;
	undefined field_0x23;
	undefined field_0x24;
	undefined field_0x25;
	undefined field_0x26;
	undefined field_0x27;
	undefined field_0x28;
	undefined field_0x29;
	undefined field_0x2a;
	undefined field_0x2b;
	undefined field_0x2c;
	undefined field_0x2d;
	undefined field_0x2e;
	undefined field_0x2f;
	undefined field_0x30;
	undefined field_0x31;
	undefined field_0x32;
	undefined field_0x33;
	undefined field_0x34;
	undefined field_0x35;
	undefined field_0x36;
	undefined field_0x37;
	undefined field_0x38;
	undefined field_0x39;
	undefined field_0x3a;
	undefined field_0x3b;
	undefined field_0x3c;
	undefined field_0x3d;
	undefined field_0x3e;
	undefined field_0x3f;
	undefined field_0x40;
	undefined field_0x41;
	undefined field_0x42;
	undefined field_0x43;
	undefined field_0x44;
	undefined field_0x45;
	undefined field_0x46;
	undefined field_0x47;
	undefined field_0x48;
	undefined field_0x49;
	undefined field_0x4a;
	undefined field_0x4b;
	undefined field_0x4c;
	undefined field_0x4d;
	undefined field_0x4e;
	undefined field_0x4f;
	undefined field_0x50;
	undefined field_0x51;
	undefined field_0x52;
	undefined field_0x53;
	undefined field_0x54;
	undefined field_0x55;
	undefined field_0x56;
	undefined field_0x57;
	undefined field_0x58;
	undefined field_0x59;
	undefined field_0x5a;
	undefined field_0x5b;
	undefined field_0x5c;
	undefined field_0x5d;
	undefined field_0x5e;
	undefined field_0x5f;
	undefined field_0x60;
	undefined field_0x61;
	undefined field_0x62;
	undefined field_0x63;
	undefined field_0x64;
	undefined field_0x65;
	undefined field_0x66;
	undefined field_0x67;
	undefined field_0x68;
	undefined field_0x69;
	undefined field_0x6a;
	undefined field_0x6b;
	undefined field_0x6c;
	undefined field_0x6d;
	undefined field_0x6e;
	undefined field_0x6f;
	undefined field_0x70;
	undefined field_0x71;
	undefined field_0x72;
	undefined field_0x73;
	undefined field_0x74;
	undefined field_0x75;
	undefined field_0x76;
	undefined field_0x77;
	undefined field_0x78;
	undefined field_0x79;
	undefined field_0x7a;
	undefined field_0x7b;
	undefined field_0x7c;
	undefined field_0x7d;
	undefined field_0x7e;
	undefined field_0x7f;
	undefined field_0x80;
	undefined field_0x81;
	undefined field_0x82;
	undefined field_0x83;
	undefined field_0x84;
	undefined field_0x85;
	undefined field_0x86;
	undefined field_0x87;
	undefined field_0x88;
	undefined field_0x89;
	undefined field_0x8a;
	undefined field_0x8b;
	undefined field_0x8c;
	undefined field_0x8d;
	undefined field_0x8e;
	undefined field_0x8f;
	undefined field_0x90;
	undefined field_0x91;
	undefined field_0x92;
	undefined field_0x93;
	undefined field_0x94;
	undefined field_0x95;
	undefined field_0x96;
	undefined field_0x97;
	undefined field_0x98;
	undefined field_0x99;
	undefined field_0x9a;
	undefined field_0x9b;
	undefined field_0x9c;
	undefined field_0x9d;
	undefined field_0x9e;
	undefined field_0x9f;
	undefined field_0xa0;
	undefined field_0xa1;
	undefined field_0xa2;
	undefined field_0xa3;
	undefined field_0xa4;
	undefined field_0xa5;
	undefined field_0xa6;
	undefined field_0xa7;
	undefined field_0xa8;
	undefined field_0xa9;
	undefined field_0xaa;
	undefined field_0xab;
	undefined field_0xac;
	undefined field_0xad;
	undefined field_0xae;
	undefined field_0xaf;
	undefined field_0xb0;
	undefined field_0xb1;
	undefined field_0xb2;
	undefined field_0xb3;
	undefined field_0xb4;
	undefined field_0xb5;
	undefined field_0xb6;
	undefined field_0xb7;
	undefined field_0xb8;
	undefined field_0xb9;
	undefined field_0xba;
	undefined field_0xbb;
	undefined field_0xbc;
	undefined field_0xbd;
	undefined field_0xbe;
	undefined field_0xbf;
	undefined field_0xc0;
	undefined field_0xc1;
	undefined field_0xc2;
	undefined field_0xc3;
	undefined field_0xc4;
	undefined field_0xc5;
	undefined field_0xc6;
	undefined field_0xc7;
	undefined field_0xc8;
	undefined field_0xc9;
	undefined field_0xca;
	undefined field_0xcb;
	undefined field_0xcc;
	undefined field_0xcd;
	undefined field_0xce;
	undefined field_0xcf;
	undefined field_0xd0;
	undefined field_0xd1;
	undefined field_0xd2;
	undefined field_0xd3;
	undefined field_0xd4;
	undefined field_0xd5;
	undefined field_0xd6;
	undefined field_0xd7;
	undefined field_0xd8;
	undefined field_0xd9;
	undefined field_0xda;
	undefined field_0xdb;
	undefined field_0xdc;
	undefined field_0xdd;
	undefined field_0xde;
	undefined field_0xdf;
	undefined field_0xe0;
	undefined field_0xe1;
	undefined field_0xe2;
	undefined field_0xe3;
	undefined field_0xe4;
	undefined field_0xe5;
	undefined field_0xe6;
	undefined field_0xe7;
	undefined field_0xe8;
	undefined field_0xe9;
	undefined field_0xea;
	undefined field_0xeb;
	undefined field_0xec;
	undefined field_0xed;
	undefined field_0xee;
	undefined field_0xef;
	undefined field_0xf0;
	undefined field_0xf1;
	undefined field_0xf2;
	undefined field_0xf3;
	undefined field_0xf4;
	undefined field_0xf5;
	undefined field_0xf6;
	undefined field_0xf7;
	undefined field_0xf8;
	undefined field_0xf9;
	undefined field_0xfa;
	undefined field_0xfb;
	undefined field_0xfc;
	undefined field_0xfd;
	undefined field_0xfe;
	undefined field_0xff;
	undefined field_0x100;
	undefined field_0x101;
	undefined field_0x102;
	undefined field_0x103;
	undefined field_0x104;
	undefined field_0x105;
	undefined field_0x106;
	undefined field_0x107;
	undefined field_0x108;
	undefined field_0x109;
	undefined field_0x10a;
	undefined field_0x10b;
	undefined field_0x10c;
	undefined field_0x10d;
	undefined field_0x10e;
	undefined field_0x10f;
	undefined field_0x110;
	undefined field_0x111;
	undefined field_0x112;
	undefined field_0x113;
	undefined field_0x114;
	undefined field_0x115;
	undefined field_0x116;
	undefined field_0x117;
	undefined field_0x118;
	undefined field_0x119;
	undefined field_0x11a;
	undefined field_0x11b;
	undefined field_0x11c;
	undefined field_0x11d;
	undefined field_0x11e;
	undefined field_0x11f;
	undefined field_0x120;
	undefined field_0x121;
	undefined field_0x122;
	undefined field_0x123;
	undefined field_0x124;
	undefined field_0x125;
	undefined field_0x126;
	undefined field_0x127;
	undefined field_0x128;
	undefined field_0x129;
	undefined field_0x12a;
	undefined field_0x12b;
	undefined field_0x12c;
	undefined field_0x12d;
	undefined field_0x12e;
	undefined field_0x12f;
	undefined field_0x130;
	undefined field_0x131;
	undefined field_0x132;
	undefined field_0x133;
	undefined field_0x134;
	undefined field_0x135;
	undefined field_0x136;
	undefined field_0x137;
	undefined field_0x138;
	undefined field_0x139;
	undefined field_0x13a;
	undefined field_0x13b;
	undefined field_0x13c;
	undefined field_0x13d;
	undefined field_0x13e;
	undefined field_0x13f;
	undefined field_0x140;
	undefined field_0x141;
	undefined field_0x142;
	undefined field_0x143;
	undefined field_0x144;
	undefined field_0x145;
	undefined field_0x146;
	undefined field_0x147;
	undefined field_0x148;
	undefined field_0x149;
	undefined field_0x14a;
	undefined field_0x14b;
	undefined field_0x14c;
	undefined field_0x14d;
	undefined field_0x14e;
	undefined field_0x14f;
	undefined field_0x150;
	undefined field_0x151;
	undefined field_0x152;
	undefined field_0x153;
	undefined field_0x154;
	undefined field_0x155;
	undefined field_0x156;
	undefined field_0x157;
	undefined field_0x158;
	undefined field_0x159;
	undefined field_0x15a;
	undefined field_0x15b;
	undefined field_0x15c;
	undefined field_0x15d;
	undefined field_0x15e;
	undefined field_0x15f;
	undefined field_0x160;
	undefined field_0x161;
	undefined field_0x162;
	undefined field_0x163;
	undefined field_0x164;
	undefined field_0x165;
	undefined field_0x166;
	undefined field_0x167;
	undefined field_0x168;
	undefined field_0x169;
	undefined field_0x16a;
	undefined field_0x16b;
	undefined field_0x16c;
	undefined field_0x16d;
	undefined field_0x16e;
	undefined field_0x16f;
	undefined field_0x170;
	undefined field_0x171;
	undefined field_0x172;
	undefined field_0x173;
	undefined field_0x174;
	undefined field_0x175;
	undefined field_0x176;
	undefined field_0x177;
	undefined field_0x178;
	undefined field_0x179;
	undefined field_0x17a;
	undefined field_0x17b;
	undefined field_0x17c;
	undefined field_0x17d;
	undefined field_0x17e;
	undefined field_0x17f;
	undefined field_0x180;
	undefined field_0x181;
	undefined field_0x182;
	undefined field_0x183;
	undefined field_0x184;
	undefined field_0x185;
	undefined field_0x186;
	undefined field_0x187;
	undefined field_0x188;
	undefined field_0x189;
	undefined field_0x18a;
	undefined field_0x18b;
	undefined field_0x18c;
	undefined field_0x18d;
	undefined field_0x18e;
	undefined field_0x18f;
	undefined field_0x190;
	undefined field_0x191;
	undefined field_0x192;
	undefined field_0x193;
	undefined field_0x194;
	undefined field_0x195;
	undefined field_0x196;
	undefined field_0x197;
	undefined field_0x198;
	undefined field_0x199;
	undefined field_0x19a;
	undefined field_0x19b;
	undefined field_0x19c;
	undefined field_0x19d;
	undefined field_0x19e;
	undefined field_0x19f;
	undefined field_0x1a0;
	undefined field_0x1a1;
	undefined field_0x1a2;
	undefined field_0x1a3;
	undefined field_0x1a4;
	undefined field_0x1a5;
	undefined field_0x1a6;
	undefined field_0x1a7;
	undefined field_0x1a8;
	undefined field_0x1a9;
	undefined field_0x1aa;
	undefined field_0x1ab;
	undefined field_0x1ac;
	undefined field_0x1ad;
	undefined field_0x1ae;
	undefined field_0x1af;
	undefined field_0x1b0;
	undefined field_0x1b1;
	undefined field_0x1b2;
	undefined field_0x1b3;
	undefined field_0x1b4;
	undefined field_0x1b5;
	undefined field_0x1b6;
	undefined field_0x1b7;
	undefined field_0x1b8;
	undefined field_0x1b9;
	undefined field_0x1ba;
	undefined field_0x1bb;
	undefined field_0x1bc;
	undefined field_0x1bd;
	undefined field_0x1be;
	undefined field_0x1bf;
	undefined field_0x1c0;
	undefined field_0x1c1;
	undefined field_0x1c2;
	undefined field_0x1c3;
	undefined field_0x1c4;
	undefined field_0x1c5;
	undefined field_0x1c6;
	undefined field_0x1c7;
	undefined field_0x1c8;
	undefined field_0x1c9;
	undefined field_0x1ca;
	undefined field_0x1cb;
	undefined field_0x1cc;
	undefined field_0x1cd;
	undefined field_0x1ce;
	undefined field_0x1cf;
	undefined field_0x1d0;
	undefined field_0x1d1;
	undefined field_0x1d2;
	undefined field_0x1d3;
	undefined field_0x1d4;
	undefined field_0x1d5;
	undefined field_0x1d6;
	undefined field_0x1d7;
	undefined field_0x1d8;
	undefined field_0x1d9;
	undefined field_0x1da;
	undefined field_0x1db;
	undefined field_0x1dc;
	undefined field_0x1dd;
	undefined field_0x1de;
	undefined field_0x1df;
	undefined field_0x1e0;
	undefined field_0x1e1;
	undefined field_0x1e2;
	undefined field_0x1e3;
	undefined field_0x1e4;
	undefined field_0x1e5;
	undefined field_0x1e6;
	undefined field_0x1e7;
	undefined field_0x1e8;
	undefined field_0x1e9;
	undefined field_0x1ea;
	undefined field_0x1eb;
	undefined field_0x1ec;
	undefined field_0x1ed;
	undefined field_0x1ee;
	undefined field_0x1ef;
	undefined field_0x1f0;
	undefined field_0x1f1;
	undefined field_0x1f2;
	undefined field_0x1f3;
	undefined field_0x1f4;
	undefined field_0x1f5;
	undefined field_0x1f6;
	undefined field_0x1f7;
	undefined field_0x1f8;
	undefined field_0x1f9;
	undefined field_0x1fa;
	undefined field_0x1fb;
	undefined field_0x1fc;
	undefined field_0x1fd;
	undefined field_0x1fe;
	undefined field_0x1ff;
	undefined field_0x200;
	undefined field_0x201;
	undefined field_0x202;
	undefined field_0x203;
	undefined field_0x204;
	undefined field_0x205;
	undefined field_0x206;
	undefined field_0x207;
	undefined field_0x208;
	undefined field_0x209;
	undefined field_0x20a;
	undefined field_0x20b;
	undefined field_0x20c;
	undefined field_0x20d;
	undefined field_0x20e;
	undefined field_0x20f;
	undefined field_0x210;
	undefined field_0x211;
	undefined field_0x212;
	undefined field_0x213;
	undefined field_0x214;
	undefined field_0x215;
	undefined field_0x216;
	undefined field_0x217;
	undefined field_0x218;
	undefined field_0x219;
	undefined field_0x21a;
	undefined field_0x21b;
	undefined field_0x21c;
	undefined field_0x21d;
	undefined field_0x21e;
	undefined field_0x21f;
	undefined field_0x220;
	undefined field_0x221;
	undefined field_0x222;
	undefined field_0x223;
	undefined field_0x224;
	undefined field_0x225;
	undefined field_0x226;
	undefined field_0x227;
	undefined field_0x228;
	undefined field_0x229;
	undefined field_0x22a;
	undefined field_0x22b;
	undefined field_0x22c;
	undefined field_0x22d;
	undefined field_0x22e;
	undefined field_0x22f;
	undefined field_0x230;
	undefined field_0x231;
	undefined field_0x232;
	undefined field_0x233;
	undefined field_0x234;
	undefined field_0x235;
	undefined field_0x236;
	undefined field_0x237;
	undefined field_0x238;
	undefined field_0x239;
	undefined field_0x23a;
	undefined field_0x23b;
	undefined field_0x23c;
	undefined field_0x23d;
	undefined field_0x23e;
	undefined field_0x23f;
	undefined field_0x240;
	undefined field_0x241;
	undefined field_0x242;
	undefined field_0x243;
	undefined field_0x244;
	undefined field_0x245;
	undefined field_0x246;
	undefined field_0x247;
	undefined field_0x248;
	undefined field_0x249;
	undefined field_0x24a;
	undefined field_0x24b;
	undefined field_0x24c;
	undefined field_0x24d;
	undefined field_0x24e;
	undefined field_0x24f;
	undefined field_0x250;
	undefined field_0x251;
	undefined field_0x252;
	undefined field_0x253;
	undefined field_0x254;
	undefined field_0x255;
	undefined field_0x256;
	undefined field_0x257;
	undefined field_0x258;
	undefined field_0x259;
	undefined field_0x25a;
	undefined field_0x25b;
	undefined field_0x25c;
	undefined field_0x25d;
	undefined field_0x25e;
	undefined field_0x25f;
	undefined field_0x260;
	undefined field_0x261;
	undefined field_0x262;
	undefined field_0x263;
	undefined field_0x264;
	undefined field_0x265;
	undefined field_0x266;
	undefined field_0x267;
	undefined field_0x268;
	undefined field_0x269;
	undefined field_0x26a;
	undefined field_0x26b;
	undefined field_0x26c;
	undefined field_0x26d;
	undefined field_0x26e;
	undefined field_0x26f;
	undefined field_0x270;
	undefined field_0x271;
	undefined field_0x272;
	undefined field_0x273;
	undefined field_0x274;
	undefined field_0x275;
	undefined field_0x276;
	undefined field_0x277;
	undefined field_0x278;
	undefined field_0x279;
	undefined field_0x27a;
	undefined field_0x27b;
	undefined field_0x27c;
	undefined field_0x27d;
	undefined field_0x27e;
	undefined field_0x27f;
	undefined field_0x280;
	undefined field_0x281;
	undefined field_0x282;
	undefined field_0x283;
	undefined field_0x284;
	undefined field_0x285;
	undefined field_0x286;
	undefined field_0x287;
	undefined field_0x288;
	undefined field_0x289;
	undefined field_0x28a;
	undefined field_0x28b;
	undefined field_0x28c;
	undefined field_0x28d;
	undefined field_0x28e;
	undefined field_0x28f;
	undefined field_0x290;
	undefined field_0x291;
	undefined field_0x292;
	undefined field_0x293;
	undefined field_0x294;
	undefined field_0x295;
	undefined field_0x296;
	undefined field_0x297;
	undefined field_0x298;
	undefined field_0x299;
	undefined field_0x29a;
	undefined field_0x29b;
	undefined field_0x29c;
	undefined field_0x29d;
	undefined field_0x29e;
	undefined field_0x29f;
	undefined field_0x2a0;
	undefined field_0x2a1;
	undefined field_0x2a2;
	undefined field_0x2a3;
	undefined field_0x2a4;
	undefined field_0x2a5;
	undefined field_0x2a6;
	undefined field_0x2a7;
	undefined field_0x2a8;
	undefined field_0x2a9;
	undefined field_0x2aa;
	undefined field_0x2ab;
	undefined field_0x2ac;
	undefined field_0x2ad;
	undefined field_0x2ae;
	undefined field_0x2af;
	undefined field_0x2b0;
	undefined field_0x2b1;
	undefined field_0x2b2;
	undefined field_0x2b3;
	undefined field_0x2b4;
	undefined field_0x2b5;
	undefined field_0x2b6;
	undefined field_0x2b7;
	undefined field_0x2b8;
	undefined field_0x2b9;
	undefined field_0x2ba;
	undefined field_0x2bb;
	undefined field_0x2bc;
	undefined field_0x2bd;
	undefined field_0x2be;
	undefined field_0x2bf;
	undefined pad_bzr_0x2c0_0x2fe[63];
	struct GameObjectClass* buildClass;
	float buildDoneTime;
	bool deployedBuilder;
	bool poweredBuilder;
	undefined field_0x2ca;
	undefined field_0x2cb;
	int powerHandle;
	struct Geizer* powerSource;
	int animHandle;
	float delayTimer;
	float nextRepair;
	enum SmokeState smoke;
	struct _OBJ76* smokeList[8];
	int smokeCount;
	float smokeTimer;
	struct SmokeEmitter* smokeEmitter;
	struct _OBJ76* ejectVehicle;
	struct _OBJ76* ejectPowerup;
};

struct hist {
	ulong dwOffset;
	ulong dwDelay;
	int BAddOffset;
	ulong dwOffLow;
	int BAddOffLow;
	ulong dwOffHigh;
	int BAddOffHigh;
};

struct NetPlayer {
	bool waitingForSpawnPoint;
	ulong offset;
	int BAddOffset;
	ulong delay;
	char flagBuf[256];
	bool bGotFlag;
	struct SpawnPoint* SpawnPointer;
	ulong dwJoinTime;
	ulong dwLaggingTime;
	ulong dwNewestRxTime;
	ulong dwNewestSendTime;
	int flagIndex;
	bool bGotSyncPacket;
	bool bGotSyncPacketAck;
	uchar bTeamNumber;
	ulong deaths;
	ulong kills;
	ushort playerId;
	char playerName[24];
	bool bClockInitialized;
	int iHistQueuePointer;
	int iHistQueueSize;
	struct hist hist[10];
	ulong dwPingReplies;
};

struct SpawnPoint {
	struct VECTOR_3D location;
	struct NetPlayer* netPlayer;
};


struct PING_PACKET {
	ulong master_send_time;
	long offset;
	ulong host_start_time;
	ulong delay;
	int BAddOffset;
};

struct Net {
	enum NetState state;
	char buffer[1024];
	long dpEnumSessionsTimeout;
	struct VECTOR_3D start_location;
	bool got_start_location;
	ulong dwLastBandWidthSendTime;
	ulong dwMaxBufferSize;
	ulong dwMaxPlayers;
	ulong dwLastPing;
	ulong dwLastSpawn;
	ulong dwNextPing;
	ulong dwNextSyncSend;
	ulong dwLastPingA[256];
	uchar bLastPingAverageCount;
	struct PING_PACKET* ping;
	ulong* ping_master_send_time;
	char pingreply_buffer[20];
	struct PINGREPLY_PACKET* pingreply;
	ulong* pingreply_slave_received_time;
	ulong* pingreply_master_send_time;
	bool bDoNetSync;
	ulong dwNextScoreTime;
	void* MessageHandler;
};

struct PINGREPLY_PACKET {
	ulong master_send_time;
	ulong slave_received_time;
};

struct ScorePlayer {
	ushort dpid;
	int iTeam;
	char caTeam[10];
	char caKills[10];
	char caTimeInZone[30];
	char caLives[10];
	int iKills;
	int iDeaths;
	float fStartTime;
	float fTimeInZone;
	int iLives;
	char caDeaths[10];
	char caStartTime[10];
	char caName[25];
};

union ModeButton_u_0 {
	long type;
	int mode;
	struct GameObjectClass* item;
	struct GameObject* unit;
};

struct ModeButton {
	union ModeButton_u_0 _u_0;
	long color;
	float healthRatio;
	float healthChange;
	float ammoRatio;
	float ammoChange;
};

struct CmdButton {
	enum AiCommand command;
	struct GameObject* object;
	struct VECTOR_3D* pos;
	long color;
};

typedef enum MenuMode {
	MENU_BUILD_PANEL = -2147352568,
	MENU_DEFENSE_PANEL = -2147352568,
	MENU_ITEM_PANEL = -2147352568,
	MENU_MODE_PANEL = -2147352568,
	MENU_NAV_PANEL = -2147352568,
	MENU_TARGET_PANEL = -2147352568,
	MENU_TYPE_PANEL = -2147352568,
	MENU_UNIT_PANEL = -2147352568
} MenuMode;

struct tagPOINTBZ { // had to rename because it conflicts with a windows api struct
	long x;
	long y;
};

struct tagRECTBZ { // same as tagRECT
	long left;
	long top;
	long right;
	long bottom;
};

struct ControlPanel {
	undefined field_0x0;
	undefined field_0x1;
	undefined field_0x2;
	undefined field_0x3;
	undefined field_0x4;
	undefined field_0x5;
	undefined field_0x6;
	undefined field_0x7;
	struct tagRECTBZ viewRect; 
	int selectList[90];
	long selectNum;
	int groupList[10][90];
	long groupNum[10];
	int targetList[10];
	long targetNum;
	float scanTimer;
	struct tagPOINTBZ holdPos; 
	float holdTime;
	int bubbleHelp;
	struct ModeButton modeButton[10];
	struct CmdButton cmdButton;
	struct CmdButton altButton;
	struct ModeList currentModes;
	int currentMode;
	long editMode;
	long satelliteEnabled;
	long currentItem;
	struct BuildItem* buildItem;
	ulong itemEnabled;
	ulong itemSelected;
	ulong itemAttacked;
	ulong itemDisplayed;
	long menuPos;
	long menuItem;
	long menuState;
	enum MenuMode menuMode;
};