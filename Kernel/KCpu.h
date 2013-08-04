#pragma once

struct __declspec(align(16)) KCpuFeatures
{
	UInt32 FamilyID;	// Processor Family
	UInt32 Model;		// Processor Model
	UInt32 TypeID;		// Processor Type
	UInt32 StepID;		// Stepping ID
	UInt32 Flags;		// Feature Flags
	UInt32 Flags2;		// More Feature Flags
	UInt32 EFlags;		// Extended Feature Flags
	UInt32 EFlags2;		// Extended Feature Flags pg2

	UInt32 PhysicalCores;
	UInt32 LogicalCores;

	char VendorName[16];	// Vendor/Creator ID
	char TypeName[20];      // cpu type
	char FamilyName[50];    // the original cpu name

	bool hasFloatingPointUnit;
	bool hasVirtual8086ModeEnhancements;
	bool hasDebuggingExtensions;
	bool hasPageSizeExtensions;
	bool hasTimeStampCounter;
	bool hasModelSpecificRegisters;
	bool hasPhysicalAddressExtension;
	bool hasCOMPXCHG8BInstruction;
	bool hasAdvancedProgrammableInterruptController;
	bool hasSEPFastSystemCall;
	bool hasMemoryTypeRangeRegisters;
	bool hasPTEGlobalFlag;
	bool hasMachineCheckArchitecture;
	bool hasConditionalMoveAndCompareInstructions;
	bool hasFGPageAttributeTable;
	bool has36bitPageSizeExtension;
	bool hasProcessorSerialNumber;
	bool hasCFLUSHInstruction;
	bool hasDebugStore;
	bool hasACPIThermalMonitorAndClockControl;
	bool hasMultimediaExtensions;
	bool hasFastStreamingSIMDExtensionsSaveRestore;
	bool hasStreamingSIMDExtensions;
	bool hasStreamingSIMD2Extensions;
	bool hasSelfSnoop;
	bool hasMultiThreading;			// is TRUE for both multi-core and Hyperthreaded CPUs.
	bool hasThermalMonitor;
	bool hasIntel64BitArchitecture;
	bool hasStreamingSIMD3Extensions;
	bool hasSupplementalStreamingSIMD3Extensions;
	bool hasStreamingSIMD4Extensions;
	bool hasStreamingSIMD4Extensions2;

	// AMD-specific CPU Features
	bool hasMultimediaExtensionsExt;
	bool hasAMD64BitArchitecture;
	bool has3DNOWInstructionExtensionsExt;
	bool has3DNOWInstructionExtensions;
	bool hasStreamingSIMD4ExtensionsA;
};

void KCpuDetectInit(KCpuFeatures& x86caps);

void KCpuIdleWait();