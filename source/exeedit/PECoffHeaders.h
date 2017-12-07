#pragma once


struct ImageDosHeader
{      
	uint16_t	e_magic;                     // Magic number
	uint16_t	e_cblp;                      // Bytes on last page of file
	uint16_t	e_cp;                        // Pages in file
	uint16_t	e_crlc;                      // Relocations
	uint16_t	e_cparhdr;                   // Size of header in paragraphs
	uint16_t	e_minalloc;                  // Minimum extra paragraphs needed
	uint16_t	e_maxalloc;                  // Maximum extra paragraphs needed
	uint16_t	e_ss;                        // Initial (relative) SS value
	uint16_t	e_sp;                        // Initial SP value
	uint16_t	e_csum;                      // Checksum
	uint16_t	e_ip;                        // Initial IP value
	uint16_t	e_cs;                        // Initial (relative) CS value
	uint16_t	e_lfarlc;                    // File address of relocation table
	uint16_t	e_ovno;                      // Overlay number
	uint16_t	e_res[4];                    // Reserved words
	uint16_t	e_oemid;                     // OEM identifier (for e_oeminfo)
	uint16_t	e_oeminfo;                   // OEM information; e_oemid specific
	uint16_t	e_res2[10];                  // Reserved words
	uint32_t	e_lfanew;                    // File address of new exe header
};


struct ImageFileHeader
{
	uint16_t	Machine;
	uint16_t	NumberOfSections;
	uint32_t	TimeDateStamp;
	uint32_t	PointerToSymbolTable;
	uint32_t	NumberOfSymbols;
	uint16_t	SizeOfOptionalHeader;
	uint16_t	Characteristics;
};


struct ImageDataDirectory
{
	uint32_t	VirtualAddress;
	uint32_t	Size;
};


struct ImageOptionalHeader
{
	// Standard fields.

	uint16_t	Magic;
	uint8_t		MajorLinkerVersion;
	uint8_t		MinorLinkerVersion;
	uint32_t	SizeOfCode;
	uint32_t	SizeOfInitializedData;
	uint32_t	SizeOfUninitializedData;
	uint32_t	AddressOfEntryPoint;
	uint32_t	BaseOfCode;
	uint32_t	BaseOfData;

	// NT additional fields.

	uint32_t	ImageBase;
	uint32_t	SectionAlignment;
	uint32_t	FileAlignment;
	uint16_t	MajorOperatingSystemVersion;
	uint16_t	MinorOperatingSystemVersion;
	uint16_t	MajorImageVersion;
	uint16_t	MinorImageVersion;
	uint16_t	MajorSubsystemVersion;
	uint16_t	MinorSubsystemVersion;
	uint32_t	Win32VersionValue;
	uint32_t	SizeOfImage;
	uint32_t	SizeOfHeaders;
	uint32_t	CheckSum;
	uint16_t	Subsystem;
	uint16_t	DllCharacteristics;
	uint32_t	SizeOfStackReserve;
	uint32_t	SizeOfStackCommit;
	uint32_t	SizeOfHeapReserve;
	uint32_t	SizeOfHeapCommit;
	uint32_t	LoaderFlags;
	uint32_t	NumberOfRvaAndSizes;
	ImageDataDirectory DataDirectory[16];
};


struct ImageNtHeaders
{
	uint32_t			Signature;
	ImageFileHeader		FileHeader;
	ImageOptionalHeader	OptionalHeader;
};


struct SectionHeader
{
	uint8_t		Name[8];
	union {
		uint32_t PhysicalAddress;
		uint32_t VirtualSize;
	} Misc;
	uint32_t	VirtualAddress;
	uint32_t	SizeOfRawData;
	uint32_t	PointerToRawData;
	uint32_t	PointerToRelocations;
	uint32_t	PointerToLinenumbers;
	uint16_t	NumberOfRelocations;
	uint16_t	NumberOfLinenumbers;
	uint32_t	Characteristics;
};

enum class SectionCharacteristics : uint32_t
{
	IMAGE_SCN_TYPE_NO_PAD				= 0x00000008
	, IMAGE_SCN_CNT_CODE				= 0x00000020
	, IMAGE_SCN_CNT_INITIALIZED_DATA	= 0x00000040
	, IMAGE_SCN_CNT_UNINITIALIZED_DATA	= 0x00000080
	, IMAGE_SCN_LNK_OTHER				= 0x00000100
	, IMAGE_SCN_LNK_INFO				= 0x00000200
	, IMAGE_SCN_LNK_REMOVE				= 0x00000800
	, IMAGE_SCN_LNK_COMDAT				= 0x00001000
	, IMAGE_SCN_GPREL					= 0x00008000
	, IMAGE_SCN_MEM_PURGEABLE			= 0x00020000
	, IMAGE_SCN_MEM_16BIT				= 0x00020000
	, IMAGE_SCN_MEM_LOCKED				= 0x00040000
	, IMAGE_SCN_MEM_PRELOAD				= 0x00080000
	, IMAGE_SCN_ALIGN_1BYTES			= 0x00100000
	, IMAGE_SCN_ALIGN_2BYTES			= 0x00200000
	, IMAGE_SCN_ALIGN_4BYTES			= 0x00300000
	, IMAGE_SCN_ALIGN_8BYTES			= 0x00400000
	, IMAGE_SCN_ALIGN_16BYTES			= 0x00500000
	, IMAGE_SCN_ALIGN_32BYTES			= 0x00600000
	, IMAGE_SCN_ALIGN_64BYTES			= 0x00700000
	, IMAGE_SCN_ALIGN_128BYTES			= 0x00800000
	, IMAGE_SCN_ALIGN_256BYTES			= 0x00900000
	, IMAGE_SCN_ALIGN_512BYTES			= 0x00A00000
	, IMAGE_SCN_ALIGN_1024BYTES			= 0x00B00000
	, IMAGE_SCN_ALIGN_2048BYTES			= 0x00C00000
	, IMAGE_SCN_ALIGN_4096BYTES			= 0x00D00000
	, IMAGE_SCN_ALIGN_8192BYTES			= 0x00E00000
	, IMAGE_SCN_LNK_NRELOC_OVFL			= 0x01000000
	, IMAGE_SCN_MEM_DISCARDABLE			= 0x02000000
	, IMAGE_SCN_MEM_NOT_CACHED			= 0x04000000
	, IMAGE_SCN_MEM_NOT_PAGED			= 0x08000000
	, IMAGE_SCN_MEM_SHARED				= 0x10000000
	, IMAGE_SCN_MEM_EXECUTE				= 0x20000000
	, IMAGE_SCN_MEM_READ				= 0x40000000
	, IMAGE_SCN_MEM_WRITE				= 0x80000000
};
