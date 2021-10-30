/***************************************************************************************************

  Zyan Disassembler Library (Zydis)

  Original Author : Mappa

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

***************************************************************************************************/

/**
 * @file
 * Functions for encoding instructions.
 */

#ifndef ZYDIS_ENCODER_H
#define ZYDIS_ENCODER_H

#include <Zycore/Types.h>
#include <Zydis/MetaInfo.h>
#include <Zydis/Register.h>
#include <Zydis/DecoderTypes.h>
#include <Zydis/Mnemonic.h>
#include <Zydis/Status.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/* Macros                                                                                         */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* Constants                                                                                      */
/* ---------------------------------------------------------------------------------------------- */

#define ZYDIS_ENCODER_MAX_OPERANDS 5

/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */
/* Enums and types                                                                                */
/* ============================================================================================== */

/**
 * Defines the `ZydisEncodableEncoding` enum.
 */
typedef enum ZydisEncodableEncoding_
{
    ZYDIS_ENCODABLE_ENCODING_DEFAULT                = 0x00000000,
    ZYDIS_ENCODABLE_ENCODING_LEGACY                 = 0x00000001,
    ZYDIS_ENCODABLE_ENCODING_3DNOW                  = 0x00000002,
    ZYDIS_ENCODABLE_ENCODING_XOP                    = 0x00000004,
    ZYDIS_ENCODABLE_ENCODING_VEX                    = 0x00000008,
    ZYDIS_ENCODABLE_ENCODING_EVEX                   = 0x00000010,
    ZYDIS_ENCODABLE_ENCODING_MVEX                   = 0x00000020,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_ENCODABLE_ENCODING_MAX_VALUE              = (ZYDIS_ENCODABLE_ENCODING_MVEX | 
                                                       (ZYDIS_ENCODABLE_ENCODING_MVEX - 1)),
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_ENCODABLE_ENCODING_REQUIRED_BITS          = 
        ZYAN_BITS_TO_REPRESENT(ZYDIS_ENCODABLE_ENCODING_MAX_VALUE)
} ZydisEncodableEncoding;

/**
 * Defines the `ZydisEncodablePrefix` enum.
 */
typedef enum ZydisEncodablePrefix_
{
    ZYDIS_ENCODABLE_PREFIX_NONE                     = 0x00000000,
    ZYDIS_ENCODABLE_PREFIX_LOCK                     = 0x00000001,
    ZYDIS_ENCODABLE_PREFIX_REP                      = 0x00000002,
    ZYDIS_ENCODABLE_PREFIX_REPE                     = 0x00000004,
    ZYDIS_ENCODABLE_PREFIX_REPZ                     = ZYDIS_ENCODABLE_PREFIX_REPE,
    ZYDIS_ENCODABLE_PREFIX_REPNE                    = 0x00000008,
    ZYDIS_ENCODABLE_PREFIX_REPNZ                    = ZYDIS_ENCODABLE_PREFIX_REPNE,
    ZYDIS_ENCODABLE_PREFIX_BND                      = 0x00000010,
    ZYDIS_ENCODABLE_PREFIX_XACQUIRE                 = 0x00000020,
    ZYDIS_ENCODABLE_PREFIX_XRELEASE                 = 0x00000040,
    ZYDIS_ENCODABLE_PREFIX_BRANCH_NOT_TAKEN         = 0x00000080,
    ZYDIS_ENCODABLE_PREFIX_BRANCH_TAKEN             = 0x00000100,
    ZYDIS_ENCODABLE_PREFIX_NOTRACK                  = 0x00000200,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_CS               = 0x00000400,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_SS               = 0x00000800,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_DS               = 0x00001000,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_ES               = 0x00002000,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_FS               = 0x00004000,
    ZYDIS_ENCODABLE_PREFIX_SEGMENT_GS               = 0x00008000,

    ZYDIS_ENCODABLE_PREFIX_SEGMENT_MASK             = (ZYDIS_ENCODABLE_PREFIX_SEGMENT_CS |
                                                       ZYDIS_ENCODABLE_PREFIX_SEGMENT_SS |
                                                       ZYDIS_ENCODABLE_PREFIX_SEGMENT_DS |
                                                       ZYDIS_ENCODABLE_PREFIX_SEGMENT_ES |
                                                       ZYDIS_ENCODABLE_PREFIX_SEGMENT_FS |
                                                       ZYDIS_ENCODABLE_PREFIX_SEGMENT_GS),

    /**
     * Maximum value of this enum.
     */
    ZYDIS_ENCODABLE_PREFIX_MAX_VALUE                = (ZYDIS_ENCODABLE_PREFIX_SEGMENT_GS |
                                                       (ZYDIS_ENCODABLE_PREFIX_SEGMENT_GS - 1)),
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_ENCODABLE_PREFIX_REQUIRED_BITS            =
        ZYAN_BITS_TO_REPRESENT(ZYDIS_ENCODABLE_PREFIX_MAX_VALUE)
} ZydisEncodablePrefix;

/**
 * Defines the `ZydisEncodableBranchType` enum.
 */
typedef enum ZydisEncodableBranchType_
{
    ZYDIS_ENCODABLE_BRANCH_TYPE_NONE,
    ZYDIS_ENCODABLE_BRANCH_TYPE_SHORT,
    ZYDIS_ENCODABLE_BRANCH_TYPE_NEAR16,
    ZYDIS_ENCODABLE_BRANCH_TYPE_NEAR32,
    ZYDIS_ENCODABLE_BRANCH_TYPE_NEAR64,
    ZYDIS_ENCODABLE_BRANCH_TYPE_FAR16,
    ZYDIS_ENCODABLE_BRANCH_TYPE_FAR32,
    ZYDIS_ENCODABLE_BRANCH_TYPE_FAR64,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_ENCODABLE_BRANCH_TYPE_MAX_VALUE = ZYDIS_ENCODABLE_BRANCH_TYPE_FAR64,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_ENCODABLE_BRANCH_TYPE_REQUIRED_BITS = 
        ZYAN_BITS_TO_REPRESENT(ZYDIS_ENCODABLE_BRANCH_TYPE_MAX_VALUE)
} ZydisEncodableBranchType;

/**
 * Defines the `ZydisAddressSizeHint` enum.
 */
typedef enum ZydisAddressSizeHint_
{
    ZYDIS_ADDRESS_SIZE_HINT_NONE,
    ZYDIS_ADDRESS_SIZE_HINT_16,
    ZYDIS_ADDRESS_SIZE_HINT_32,
    ZYDIS_ADDRESS_SIZE_HINT_64,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_ADDRESS_SIZE_MAX_VALUE = ZYDIS_ADDRESS_SIZE_HINT_64,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_ADDRESS_SIZE_MAX_REQUIRED_BITS = 
        ZYAN_BITS_TO_REPRESENT(ZYDIS_ADDRESS_SIZE_MAX_VALUE)
} ZydisAddressSizeHint;

/**
 * Defines the `ZydisOperandSizeHint` enum.
 */
typedef enum ZydisOperandSizeHint_
{
    ZYDIS_OPERAND_SIZE_HINT_NONE,
    ZYDIS_OPERAND_SIZE_HINT_8,
    ZYDIS_OPERAND_SIZE_HINT_16,
    ZYDIS_OPERAND_SIZE_HINT_32,
    ZYDIS_OPERAND_SIZE_HINT_64,

    /**
     * Maximum value of this enum.
     */
    ZYDIS_OPERAND_SIZE_MAX_VALUE = ZYDIS_OPERAND_SIZE_HINT_64,
    /**
     * The minimum number of bits required to represent all values of this enum.
     */
    ZYDIS_OPERAND_SIZE_REQUIRED_BITS = 
        ZYAN_BITS_TO_REPRESENT(ZYDIS_OPERAND_SIZE_MAX_VALUE)
} ZydisOperandSizeHint;

/**
 * Defines the `ZydisEncoderOperand` struct.
 */
typedef struct ZydisEncoderOperand_
{
    /**
     * The type of the operand.
     */
    ZydisOperandType type;
    /**
     * Extended info for register-operands.
     */
    struct ZydisEncoderOperandReg_
    {
        /**
         * The register value.
         */
        ZydisRegister value;
        /**
         * Is this 4th operand (VEX/XOP).
         */
        ZyanBool is4;
    } reg;
    /**
     * Extended info for memory-operands.
     */
    struct ZydisEncoderOperandMem_
    {
        /**
         * The base register.
         */
        ZydisRegister base;
        /**
         * The index register.
         */
        ZydisRegister index;
        /**
         * The scale factor.
         */
        ZyanU8 scale;
        /**
         * The displacement value.
         */
        ZyanI64 displacement;
        /**
         * Size of this operand in bytes.
         */
        ZyanU16 size;
    } mem;
    /**
     * Extended info for pointer-operands.
     */
    struct ZydisEncoderOperandPtr_
    {
        /**
         * The segment value.
         */
        ZyanU16 segment;
        /**
         * The offset value.
         */
        ZyanU32 offset;
    } ptr;
    /**
     * Extended info for immediate-operands.
     */
    union ZydisEncoderOperandImm_
    {
        /**
         * The unsigned immediate value.
         */
        ZyanU64 u;
        /**
         * The signed immediate value.
         */
        ZyanI64 s;
    } imm;
} ZydisEncoderOperand;

/**
 * Defines the `ZydisEncoderRequest` struct.
 */
typedef struct ZydisEncoderRequest_
{
    /**
     * The machine mode used to decode this instruction.
     */
    ZydisMachineMode machine_mode;
    /**
     * This optional field can be used to restrict allowed encodings for desired instruction.
     */
    ZydisEncodableEncoding allowed_encodings;
    /**
     * The instruction-mnemonic.
     */
    ZydisMnemonic mnemonic;
    /**
     * A combination of requested encodable prefixes for desired instruction.
     */
    ZydisEncodablePrefix prefixes;
    /**
     * Branch type (required for branching instructions only).
     */
    ZydisEncodableBranchType branch_type;
    /**
     * Optional address size hint used to resolve ambiguities for some instructions.
     */
    ZydisAddressSizeHint address_size_hint;
    /**
     * Optional operand size hint used to resolve ambiguities for some instructions.
     */
    ZydisOperandSizeHint operand_size_hint;
    /**
     * The number of instruction-operands.
     */
    ZyanU8 operand_count;
    /**
     * Detailed info for all explicit and implicit instruction operands.
     */
    ZydisEncoderOperand operands[ZYDIS_ENCODER_MAX_OPERANDS];
    /**
     * Extended info for `EVEX` instructions.
     */
    struct ZydisEncoderRequestEvexFeatures_
    {
        /**
         * The broadcast-mode.
         */
        ZydisBroadcastMode broadcast;
        /**
         * The rounding-mode.
         */
        ZydisRoundingMode rounding;
        /**
         * Signals, if the `SAE` (suppress-all-exceptions) functionality should be enabled for 
         * the instruction.
         */
        ZyanBool sae;
        /**
         * Signals, if the zeroing-mask functionality should be enabled for the instruction.
         */
        ZyanBool zeroing_mask;
    } evex;
    /**
     * Extended info for `MVEX` instructions.
     */
    struct ZydisEncoderRequestMvexFeatures_
    {
        /**
         * The broadcast-mode.
         */
        ZydisBroadcastMode broadcast;
        /**
         * The data-conversion mode.
         */
        ZydisConversionMode conversion;
        /**
         * The rounding-mode.
         */
        ZydisRoundingMode rounding;
        /**
         * The `AVX` register-swizzle mode.
         */
        ZydisSwizzleMode swizzle;
        /**
         * Signals, if the `SAE` (suppress-all-exceptions) functionality is enabled for
         * the instruction.
         */
        ZyanBool sae;
        /**
         * Signals, if the instruction has a memory-eviction-hint (`KNC` only).
         */
        ZyanBool eviction_hint;
    } mvex;
} ZydisEncoderRequest;

/* ============================================================================================== */
/* Exported functions                                                                             */
/* ============================================================================================== */

/**
 * @addtogroup encoder Encoder
 * Functions allowing encoding of instruction bytes from a machine interpretable struct.
 * @{
 */

/**
 * Decodes the instruction in the given input `buffer`.
 *
 * @param   request     A pointer to the `ZydisEncoderRequest` struct.
 * @param   buffer      A pointer to the output buffer receiving encoded instruction.
 * @param   length      A pointer to the variable containing length of the output buffer. Upon 
 *                      successful return this variable receives length of the encoded instruction.
 *
 * @return  A zyan status code.
 */
ZYDIS_EXPORT ZyanStatus ZydisEncoderEncodeInstruction(const ZydisEncoderRequest *request, 
    void *buffer, ZyanUSize *length);

/**
 * Decodes the instruction in the given input `buffer`.
 *
 * @param   instruction A pointer to the `ZydisDecodedInstruction` struct.
 * @param   request     A pointer to the `ZydisEncoderRequest` struct, that receives information 
 *                      necessary for encoder to re-encode the instruction. This function 
 *                      performs simple structure conversion and does minimal sanity checks on the 
 *                      input. There's no guarantee that produced request will be accepted by
 *                      `ZydisEncoderEncodeInstruction` if malformed `ZydisDecodedInstruction` is 
 *                      passed to this function.
 *
 * @return  A zyan status code.
 */
ZYDIS_EXPORT ZyanStatus ZydisEncoderDecodedInstructionToEncoderRequest(
    const ZydisDecodedInstruction *instruction, ZydisEncoderRequest *request);

/** @} */

/* ============================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* ZYDIS_ENCODER_H */