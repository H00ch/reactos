/*
 * PROJECT:   Registry manipulation library
 * LICENSE:   GPL - See COPYING in the top level directory
 * COPYRIGHT: Copyright 2005 Filip Navara <navaraf@reactos.org>
 *            Copyright 2001 - 2005 Eric Kohl
 */

#pragma once

//
// Key Types
//
#define CM_KEY_INDEX_ROOT               0x6972  // "ri"
#define CM_KEY_INDEX_LEAF               0x696C  // "li"
#define CM_KEY_FAST_LEAF                0x666C  // "lf"
#define CM_KEY_HASH_LEAF                0x686C  // "lh"

//
// Key Signatures
//
#define CM_KEY_NODE_SIGNATURE           0x6B6E  // "nk"
#define CM_LINK_NODE_SIGNATURE          0x6B6C  // "lk"
#define CM_KEY_SECURITY_SIGNATURE       0x6B73  // "sk"
#define CM_KEY_VALUE_SIGNATURE          0x6B76  // "vk"
#define CM_BIG_DATA_SIGNATURE           0x6264  // "db"

//
// CM_KEY_NODE Flags
//
#define KEY_IS_VOLATILE                 0x0001
#define KEY_HIVE_EXIT                   0x0002
#define KEY_HIVE_ENTRY                  0x0004
#define KEY_NO_DELETE                   0x0008
#define KEY_SYM_LINK                    0x0010
#define KEY_COMP_NAME                   0x0020
#define KEY_PREFEF_HANDLE               0x0040
#define KEY_VIRT_MIRRORED               0x0080
#define KEY_VIRT_TARGET                 0x0100
#define KEY_VIRTUAL_STORE               0x0200

//
// CM_KEY_VALUE Flags
//
#define VALUE_COMP_NAME                 0x0001

//
// CM_KEY_VALUE Types
//
#define CM_KEY_VALUE_SMALL              0x4
#define CM_KEY_VALUE_BIG                0x3FD8
#define CM_KEY_VALUE_SPECIAL_SIZE       0x80000000

#include <pshpack1.h>

//
// For memory-mapped Hives
//
typedef struct _CM_VIEW_OF_FILE
{
    LIST_ENTRY LRUViewList;
    LIST_ENTRY PinViewList;
    ULONG FileOffset;
    ULONG Size;
    PULONG_PTR ViewAddress;
    PVOID Bcb;
    ULONG UseCount;
} CM_VIEW_OF_FILE, *PCM_VIEW_OF_FILE;

//
// Children of Key Nodes
//
typedef struct _CHILD_LIST
{
    ULONG Count;
    HCELL_INDEX List;
} CHILD_LIST, *PCHILD_LIST;

//
// Node Key Reference to Parents
//
typedef struct  _CM_KEY_REFERENCE
{
    HCELL_INDEX KeyCell;
    PHHIVE KeyHive;
} CM_KEY_REFERENCE, *PCM_KEY_REFERENCE;

//
// Node Key
//
typedef struct _CM_KEY_NODE
{
    USHORT Signature;
    USHORT Flags;
    LARGE_INTEGER LastWriteTime;
    ULONG Spare;
    HCELL_INDEX Parent;
    ULONG SubKeyCounts[HTYPE_COUNT];
    union
    {
        struct
        {
            HCELL_INDEX SubKeyLists[HTYPE_COUNT];
            CHILD_LIST ValueList;
        };
        CM_KEY_REFERENCE ChildHiveReference;
    };
    HCELL_INDEX Security;
    HCELL_INDEX Class;
    ULONG MaxNameLen;
    ULONG MaxClassLen;
    ULONG MaxValueNameLen;
    ULONG MaxValueDataLen;
    ULONG WorkVar;
    USHORT NameLength;
    USHORT ClassLength;
    WCHAR Name[ANYSIZE_ARRAY];
} CM_KEY_NODE, *PCM_KEY_NODE;

//
// Value Key
//
typedef struct _CM_KEY_VALUE
{
    USHORT Signature;
    USHORT NameLength;
    ULONG DataLength;
    HCELL_INDEX Data;
    ULONG Type;
    USHORT Flags;
    USHORT Spare;
    WCHAR Name[ANYSIZE_ARRAY];
} CM_KEY_VALUE, *PCM_KEY_VALUE;

//
// Security Key
//
typedef struct _CM_KEY_SECURITY
{
    USHORT Signature;
    USHORT Reserved;
    HCELL_INDEX Flink;
    HCELL_INDEX Blink;
    ULONG ReferenceCount;
    ULONG DescriptorLength;
    SECURITY_DESCRIPTOR_RELATIVE Descriptor;
} CM_KEY_SECURITY, *PCM_KEY_SECURITY;

//
// Big Value Key
//
typedef struct _CM_BIG_DATA
{
    USHORT Signature;
    USHORT Count;
    HCELL_INDEX List;
} CM_BIG_DATA, *PCM_BIG_DATA;

#include <poppack.h>

//
// Generic Index Entry
//
typedef struct _CM_INDEX
{
    HCELL_INDEX Cell;
    union
    {
        UCHAR NameHint[4];
        ULONG HashKey;
    };
} CM_INDEX, *PCM_INDEX;

//
// Key Index
//
typedef struct _CM_KEY_INDEX
{
    USHORT Signature;
    USHORT Count;
    HCELL_INDEX List[ANYSIZE_ARRAY];
} CM_KEY_INDEX, *PCM_KEY_INDEX;

//
// Fast/Hash Key Index
//
typedef struct _CM_KEY_FAST_INDEX
{
    USHORT Signature;
    USHORT Count;
    CM_INDEX List[ANYSIZE_ARRAY];
} CM_KEY_FAST_INDEX, *PCM_KEY_FAST_INDEX;

//
// Cell Data
//
typedef struct _CELL_DATA
{
    union
    {
        CM_KEY_NODE KeyNode;
        CM_KEY_VALUE KeyValue;
        CM_KEY_SECURITY KeySecurity;
        CM_KEY_INDEX KeyIndex;
        CM_BIG_DATA ValueData;
        HCELL_INDEX KeyList[ANYSIZE_ARRAY];
        WCHAR KeyString[ANYSIZE_ARRAY];
    } u;
} CELL_DATA, *PCELL_DATA;
