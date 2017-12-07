#pragma once

enum class BranchType
{
	CALL,
	JMP,
	JA,
	JAE,
	JB,
	JBE,
// 	JCXZ,
// 	JECXZ,
// 	JRCXZ,
	JG,
	JGE,
	JL,
	JLE,
	JNO,
	JNP,
	JNS,
	JNZ,
	JO,
	JP,
	JS,
	JZ

	// pseudo instructions
	// I_JB,
	// I_JZ,
	// I_JNA,
	// I_JNAE,
	// I_JNB,
	// I_JNBE,
	// I_JNC,
	// I_JNE,
	// I_JNG,
	// I_JNGE,
	// I_JNL,
	// I_JNLE,
	// I_JPE ,
	// I_JPO ,
};