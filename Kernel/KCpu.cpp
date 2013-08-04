/*  Based on Cpudetection lib
 *  Copyright (C) 2002-2009  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Kernel.h"

struct BaseRegs
{
	UInt32 EAX;
	UInt32 ECX;
	UInt32 EDX;
	UInt32 EBX;
};

// Implemented in asm
SInt32 KCpuId(UInt32 cmd, BaseRegs& regs);

void KCpuDetectInit(KCpuFeatures& x86caps)
{
   BaseRegs regs;
   UInt32 MaxCpuIdCommandId;
   int cputype=0;            // Cpu type
   //AMD 64 STUFF
   UInt32 x86_64_8BITBRANDID;
   UInt32 x86_64_12BITBRANDID;

   KMemZero(x86caps.VendorName, sizeof( x86caps.VendorName) );
   x86caps.FamilyID = 0;
   x86caps.Model  = 0;
   x86caps.TypeID  = 0;
   x86caps.StepID = 0;
   x86caps.Flags  = 0;
   x86caps.EFlags = 0;

   if ( KCpuId( 0, regs ) == -1 ) return;

   MaxCpuIdCommandId = regs.EAX;
   ((UInt32*)x86caps.VendorName)[ 0 ] = regs.ECX;
   ((UInt32*)x86caps.VendorName)[ 1 ] = regs.EDX;
   ((UInt32*)x86caps.VendorName)[ 2 ] = regs.EDX;

   UInt32 LogicalCoresPerPhysicalCPU = 0;
   UInt32 PhysicalCoresPerPhysicalCPU = 1;

   if ( MaxCpuIdCommandId >= 0x00000001 )
   {
      if ( KCpuId( 0x00000001, regs ) != -1 )
      {
         x86caps.StepID   =  regs.EAX        & 0xf;
         x86caps.Model    = (regs.EAX >>  4) & 0xf;
         x86caps.FamilyID = (regs.EAX >>  8) & 0xf;
         x86caps.TypeID   = (regs.EAX >> 12) & 0x3;
		 LogicalCoresPerPhysicalCPU = ( regs.ECX >> 16 ) & 0xff;
         x86_64_8BITBRANDID = regs.ECX & 0xff;
         x86caps.Flags  =  regs.EDX;
         x86caps.Flags2 =  regs.EDX;
      }
   }

   /* detect multicore for intel cpu */
   if ((MaxCpuIdCommandId >= 0x00000004) && !KStrCmpA("GenuineIntel",x86caps.VendorName))
   {
      if ( KCpuId( 0x00000004, regs ) != -1 )
      {
         PhysicalCoresPerPhysicalCPU += ( regs.EAX >> 26) & 0x3f;
      }
   }

   if ( KCpuId( 0x80000000, regs ) != -1 )
   {
      MaxCpuIdCommandId = regs.EAX;
      if ( MaxCpuIdCommandId >= 0x80000001 )
      {
		 if ( KCpuId( 0x80000001, regs ) != -1 )
         {
			x86_64_12BITBRANDID = regs.ECX & 0xfff;
			x86caps.EFlags2 = regs.EDX;
            x86caps.EFlags = regs.EDX;

         }
      }

      /* detect multicore for amd cpu */
      if ((MaxCpuIdCommandId >= 0x80000008) && !KStrCmpA("AuthenticAMD",x86caps.VendorName))
      {
         if ( KCpuId( 0x80000008, regs ) != -1 )
         {
            PhysicalCoresPerPhysicalCPU += ( regs.EDX ) & 0xff;
         }
      }
   }

	switch(x86caps.TypeID)
	{
		case 0:
			KStrCpyA( x86caps.TypeName, "Standard OEM");
		break;
		case 1:
			KStrCpyA( x86caps.TypeName, "Overdrive");
		break;
		case 2:
			KStrCpyA( x86caps.TypeName, "Dual");
		break;
		case 3:
			KStrCpyA( x86caps.TypeName, "Reserved");
		break;
		default:
			KStrCpyA( x86caps.TypeName, "Unknown");
		break;
	}

	if ( x86caps.VendorName[ 0 ] == 'G' ) { cputype = 0; }//trick lines but if you know a way better ;p
	if ( x86caps.VendorName[ 0 ] == 'A' ) { cputype = 1; }

	KMemZero(x86caps.FamilyName, sizeof(x86caps.FamilyName));
	KCpuId(0x80000002, *(BaseRegs*)(x86caps.FamilyName   ));
	KCpuId(0x80000003, *(BaseRegs*)(x86caps.FamilyName+16));
	KCpuId(0x80000004, *(BaseRegs*)(x86caps.FamilyName+32));

	// Capability Flags
	x86caps.hasFloatingPointUnit                         = ( x86caps.Flags >>  0 ) & 1;
	x86caps.hasVirtual8086ModeEnhancements               = ( x86caps.Flags >>  1 ) & 1;
	x86caps.hasDebuggingExtensions                       = ( x86caps.Flags >>  2 ) & 1;
	x86caps.hasPageSizeExtensions                        = ( x86caps.Flags >>  3 ) & 1;
	x86caps.hasTimeStampCounter                          = ( x86caps.Flags >>  4 ) & 1;
	x86caps.hasModelSpecificRegisters                    = ( x86caps.Flags >>  5 ) & 1;
	x86caps.hasPhysicalAddressExtension                  = ( x86caps.Flags >>  6 ) & 1;
	x86caps.hasMachineCheckArchitecture                  = ( x86caps.Flags >>  7 ) & 1;
	x86caps.hasCOMPXCHG8BInstruction                     = ( x86caps.Flags >>  8 ) & 1;
	x86caps.hasAdvancedProgrammableInterruptController   = ( x86caps.Flags >>  9 ) & 1;
	x86caps.hasSEPFastSystemCall                         = ( x86caps.Flags >> 11 ) & 1;
	x86caps.hasMemoryTypeRangeRegisters                  = ( x86caps.Flags >> 12 ) & 1;
	x86caps.hasPTEGlobalFlag                             = ( x86caps.Flags >> 13 ) & 1;
	x86caps.hasMachineCheckArchitecture                  = ( x86caps.Flags >> 14 ) & 1;
	x86caps.hasConditionalMoveAndCompareInstructions     = ( x86caps.Flags >> 15 ) & 1;
	x86caps.hasFGPageAttributeTable                      = ( x86caps.Flags >> 16 ) & 1;
	x86caps.has36bitPageSizeExtension                    = ( x86caps.Flags >> 17 ) & 1;
	x86caps.hasProcessorSerialNumber                     = ( x86caps.Flags >> 18 ) & 1;
	x86caps.hasCFLUSHInstruction                         = ( x86caps.Flags >> 19 ) & 1;
	x86caps.hasDebugStore                                = ( x86caps.Flags >> 21 ) & 1;
	x86caps.hasACPIThermalMonitorAndClockControl         = ( x86caps.Flags >> 22 ) & 1;
	x86caps.hasMultimediaExtensions                      = ( x86caps.Flags >> 23 ) & 1; //mmx
	x86caps.hasFastStreamingSIMDExtensionsSaveRestore    = ( x86caps.Flags >> 24 ) & 1;
	x86caps.hasStreamingSIMDExtensions                   = ( x86caps.Flags >> 25 ) & 1; //sse
	x86caps.hasStreamingSIMD2Extensions                  = ( x86caps.Flags >> 26 ) & 1; //sse2
	x86caps.hasSelfSnoop                                 = ( x86caps.Flags >> 27 ) & 1;
	x86caps.hasMultiThreading                            = ( x86caps.Flags >> 28 ) & 1;
	x86caps.hasThermalMonitor                            = ( x86caps.Flags >> 29 ) & 1;
	x86caps.hasIntel64BitArchitecture                    = ( x86caps.Flags >> 30 ) & 1;

	// AMD-specific Flags
	x86caps.hasMultimediaExtensionsExt                   = ( x86caps.EFlags >> 22 ) & 1; //mmx2
	x86caps.hasAMD64BitArchitecture                      = ( x86caps.EFlags >> 29 ) & 1; //64bit cpu
	x86caps.has3DNOWInstructionExtensionsExt             = ( x86caps.EFlags >> 30 ) & 1; //3dnow+
	x86caps.has3DNOWInstructionExtensions                = ( x86caps.EFlags >> 31 ) & 1; //3dnow
	x86caps.hasStreamingSIMD4ExtensionsA                 = ( x86caps.EFlags2 >> 6 ) & 1; //INSERTQ / EXTRQ / MOVNT

	// --> SSE3 / SSSE3 / SSE4.1 / SSE 4.2 detection <--

	x86caps.hasStreamingSIMD3Extensions  = ( x86caps.Flags2 >> 0 ) & 1; //sse3
	x86caps.hasSupplementalStreamingSIMD3Extensions = ( x86caps.Flags2 >> 9 ) & 1; //ssse3
	x86caps.hasStreamingSIMD4Extensions  = ( x86caps.Flags2 >> 19 ) & 1; //sse4.1
	x86caps.hasStreamingSIMD4Extensions2 = ( x86caps.Flags2 >> 20 ) & 1; //sse4.2

}

