/** @file
  This file declares Sec Platform Information PPI.

  This service is the primary handoff state into the PEI Foundation.
  The Security (SEC) component creates the early, transitory memory
  environment and also encapsulates knowledge of at least the
  location of the Boot Firmware Volume (BFV).

Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

  @par Revision Reference:
  This PPI is introduced in PI Version 1.0.

**/

#ifndef __SEC_PLATFORM_INFORMATION_PPI_H__
#define __SEC_PLATFORM_INFORMATION_PPI_H__

#include "../Pi/PiPeiCis.h"

#define EFI_SEC_PLATFORM_INFORMATION_GUID \
  { \
    0x6f8c2b35, 0xfef4, 0x448d, {0x82, 0x56, 0xe1, 0x1b, 0x19, 0xd6, 0x10, 0x77 } \
  }

typedef struct _EFI_SEC_PLATFORM_INFORMATION_PPI EFI_SEC_PLATFORM_INFORMATION_PPI;

///
/// EFI_HEALTH_FLAGS
/// Contains information generated by microcode, hardware, and/or the Itanium
/// processor PAL code about the state of the processor upon reset.
///
typedef union {
  struct {
    ///
    /// A 2-bit field indicating self-test state after reset.
    ///
    UINT32    Status                   : 2;
    ///
    /// A 1-bit field indicating whether testing has occurred.
    /// If this field is zero, the processor has not been tested,
    /// and no further fields in the self-test State parameter are valid.
    ///
    UINT32    Tested                   : 1;
    ///
    /// Reserved 13 bits.
    ///
    UINT32    Reserved1                : 13;
    ///
    /// A 1-bit field. If set to 1, this indicates that virtual
    /// memory features are not available.
    ///
    UINT32    VirtualMemoryUnavailable : 1;
    ///
    /// A 1-bit field. If set to 1, this indicates that IA-32 execution
    /// is not available.
    ///
    UINT32    Ia32ExecutionUnavailable : 1;
    ///
    /// A 1-bit field. If set to 1, this indicates that the floating
    /// point unit is not available.
    ///
    UINT32    FloatingPointUnavailable : 1;
    ///
    /// A 1-bit field. If set to 1, this indicates miscellaneous
    /// functional failure other than vm, ia, or fp.
    /// The test status field provides additional information on
    /// test failures when the State field returns a value of
    /// performance restricted or functionally restricted.
    /// The value returned is implementation dependent.
    ///
    UINT32    MiscFeaturesUnavailable : 1;
    ///
    /// Reserved 12 bits.
    ///
    UINT32    Reserved2               : 12;
  } Bits;
  UINT32    Uint32;
} EFI_HEALTH_FLAGS;

#define NORMAL_BOOT_CALL     0x0
#define RECOVERY_CHECK_CALL  0x3

typedef EFI_HEALTH_FLAGS X64_HANDOFF_STATUS;
typedef EFI_HEALTH_FLAGS IA32_HANDOFF_STATUS;
///
/// The hand-off status structure for Itanium architecture.
///
typedef struct {
  ///
  /// SALE_ENTRY state : 3 = Recovery_Check
  /// and 0 = RESET or Normal_Boot phase.
  ///
  UINT8     BootPhase;
  ///
  /// Firmware status on entry to SALE.
  ///
  UINT8     FWStatus;
  UINT16    Reserved1;
  UINT32    Reserved2;
  ///
  /// Geographically significant unique processor ID assigned by PAL.
  ///
  UINT16    ProcId;
  UINT16    Reserved3;
  UINT8     IdMask;
  UINT8     EidMask;
  UINT16    Reserved4;
  ///
  /// Address to make PAL calls.
  ///
  UINT64    PalCallAddress;
  ///
  /// If the entry state is RECOVERY_CHECK, this contains the PAL_RESET
  /// return address, and if entry state is RESET, this contains
  /// address for PAL_authentication call.
  ///
  UINT64    PalSpecialAddress;
  ///
  /// GR35 from PALE_EXIT state.
  ///
  UINT64    SelfTestStatus;
  ///
  /// GR37 from PALE_EXIT state.
  ///
  UINT64    SelfTestControl;
  UINT64    MemoryBufferRequired;
} ITANIUM_HANDOFF_STATUS;

///
/// EFI_SEC_PLATFORM_INFORMATION_RECORD.
///
typedef union {
  IA32_HANDOFF_STATUS       IA32HealthFlags;
  X64_HANDOFF_STATUS        x64HealthFlags;
  ITANIUM_HANDOFF_STATUS    ItaniumHealthFlags;
} EFI_SEC_PLATFORM_INFORMATION_RECORD;

/**
  This interface conveys state information out of the Security (SEC) phase into PEI.

  This service is published by the SEC phase. The SEC phase handoff has an optional
  EFI_PEI_PPI_DESCRIPTOR list as its final argument when control is passed from SEC into the
  PEI Foundation. As such, if the platform supports the built-in self test (BIST) on IA-32 Intel
  architecture or the PAL-A handoff state for Itanium architecture, this information is encapsulated
  into the data structure abstracted by this service. This information is collected for the boot-strap
  processor (BSP) on IA-32. For Itanium architecture, it is available on all processors that execute
  the PEI Foundation.

  @param  PeiServices               The pointer to the PEI Services Table.
  @param  StructureSize             The pointer to the variable describing size of the input buffer.
  @param  PlatformInformationRecord The pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD.

  @retval EFI_SUCCESS               The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL      The buffer was too small. The current buffer size needed to
                                    hold the record is returned in StructureSize.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SEC_PLATFORM_INFORMATION)(
  IN CONST  EFI_PEI_SERVICES                    **PeiServices,
  IN OUT    UINT64                              *StructureSize,
  OUT       EFI_SEC_PLATFORM_INFORMATION_RECORD *PlatformInformationRecord
  );

///
/// This service abstracts platform-specific information. It is necessary
/// to convey this information to the PEI Foundation so that it can
/// discover where to begin dispatching PEIMs.
///
struct _EFI_SEC_PLATFORM_INFORMATION_PPI {
  EFI_SEC_PLATFORM_INFORMATION    PlatformInformation;
};

extern EFI_GUID  gEfiSecPlatformInformationPpiGuid;

#endif