#include "toGLSLInstruction.h"
#include "toGLSLOperand.h"
#include "bstrlib.h"
#include "stdio.h"

#include <assert.h>
#define ASSERT(x) assert(x)

extern void AddIndentation(HLSLCrossCompilerContext* psContext);

void CallHLSLOpcodeFunc1(HLSLCrossCompilerContext* psContext, const char* name, Instruction* psInst)
{
    bstring glsl = psContext->glsl;

    AddIndentation(psContext);
    bcatcstr(glsl, name);
    bcatcstr(glsl, "(");
    TranslateOperand(psContext, &psInst->asOperands[0]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[1]);
    bcatcstr(glsl, ");\n");
}

void CallHLSLOpcodeFunc2(HLSLCrossCompilerContext* psContext, const char* name, Instruction* psInst)
{
    bstring glsl = psContext->glsl;
    AddIndentation(psContext);
    bcatcstr(glsl, name);
    bcatcstr(glsl, "(");
    TranslateOperand(psContext, &psInst->asOperands[0]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[1]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[2]);
    bcatcstr(glsl, ");\n");
}

void CallHLSLOpcodeFunc3(HLSLCrossCompilerContext* psContext, const char* name, Instruction* psInst)
{
    bstring glsl = psContext->glsl;
    AddIndentation(psContext);
    bcatcstr(glsl, name);
    bcatcstr(glsl, "(");
    TranslateOperand(psContext, &psInst->asOperands[0]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[1]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[2]);
    bcatcstr(glsl, ", ");
    TranslateOperand(psContext, &psInst->asOperands[3]);
    bcatcstr(glsl, ");\n");
}

void TranslateInstruction(HLSLCrossCompilerContext* psContext, Instruction* psInst)
{
    bstring glsl = psContext->glsl;
    switch(psInst->eOpcode)
    {
        case OPCODE_MOV:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MOV\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = vec4(");


            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ")");
            TranslateOperandSwizzle(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ";\n");
            break;
        }
        case OPCODE_UTOF://unsigned to float
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//UTOF\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = vec4(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ")");
            TranslateOperandSwizzle(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ";\n");
            break;
        }
        case OPCODE_MAD:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MAD\n");
            CallHLSLOpcodeFunc3(psContext, "HLSL_mad", psInst);
            break;
        }
        case OPCODE_IADD:
        {
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//IADD\n");
#endif
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = ");

            bcatcstr(glsl, "vec4(");

            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, " + ");
            TranslateOperand(psContext, &psInst->asOperands[2]);

            bcatcstr(glsl, ")");
            TranslateOperandSwizzle(psContext, &psInst->asOperands[0]);

            bcatcstr(glsl, ";\n");
            break;
        }
        case OPCODE_ADD:
        {
            //Limit src swizzles based on dest swizzle
            //e.g. given hlsl asm: add r0.xy, v0.xyxx, l(0.100000, 0.000000, 0.000000, 0.000000)
            //the two sources must become vec2
            //Temp0.xy = Input0.xyxx + vec4(0.100000, 0.000000, 0.000000, 0.000000);
            //becomes
            //Temp0.xy = vec4(Input0.xyxx + vec4(0.100000, 0.000000, 0.000000, 0.000000)).xy;

            AddIndentation(psContext);
            bcatcstr(glsl, "//ADD\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = ");

            bcatcstr(glsl, "vec4(");

            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, " + ");
            TranslateOperand(psContext, &psInst->asOperands[2]);

            bcatcstr(glsl, ")");
            TranslateOperandSwizzle(psContext, &psInst->asOperands[0]);

            bcatcstr(glsl, ";\n");
            break;
        }
        case OPCODE_OR:
        {
            /*Todo: vector version */
            AddIndentation(psContext);
            bcatcstr(glsl, "//OR\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = int(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ") | int(");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_AND:
        {
            /* Todo: vector version*/
            AddIndentation(psContext);
            bcatcstr(glsl, "//AND\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = int(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ") & int(");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_GE:
        {
            /*
                dest = vec4(greaterThanEqual(vec4(srcA), vec4(srcB));
                Caveat: The result is a boolean but HLSL asm returns 0xFFFFFFFF/0x0 instead.
             */
            AddIndentation(psContext);
            bcatcstr(glsl, "//GE\n");
            CallHLSLOpcodeFunc2(psContext, "HLSL_ge", psInst);
            break;
        }
        case OPCODE_MUL:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MUL\n");
            CallHLSLOpcodeFunc2(psContext, "HLSL_mul", psInst);
            break;
        }
        case OPCODE_DIV:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//DIV\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = ");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, " / ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ";\n");
            break;
        }
        case OPCODE_SINCOS:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//SINCOS\n");
            if(psInst->asOperands[0].eType != OPERAND_TYPE_NULL)
            {
                AddIndentation(psContext);
                TranslateOperand(psContext, &psInst->asOperands[0]);//Dest sin
                bcatcstr(glsl, " = sin(");
                TranslateOperand(psContext, &psInst->asOperands[2]);//angle
                bcatcstr(glsl, ");\n");
            }

            if(psInst->asOperands[1].eType != OPERAND_TYPE_NULL)
            {
                AddIndentation(psContext);
                TranslateOperand(psContext, &psInst->asOperands[1]);//Dest cos
                bcatcstr(glsl, " = cos(");
                TranslateOperand(psContext, &psInst->asOperands[2]);//angle
                bcatcstr(glsl, ");\n");
            }
            break;
        }

        case OPCODE_DP2:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//DP2\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = dot((");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ").xy);\n");
            break;
        }
        case OPCODE_DP3:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//DP3\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = dot((");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ").xyz);\n");
            break;
        }
        case OPCODE_DP4:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//DP4\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = dot(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_NE:
        {
            //Scalar version. Use any() for vector with scalar 1
            AddIndentation(psContext);
            bcatcstr(glsl, "//NE\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = (");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, " != ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ") ? 1 : 0;\n");
            break;
        }
        case OPCODE_IGE:
        {
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//IGE\n");
#endif
            CallHLSLOpcodeFunc2(psContext, "HLSL_ige", psInst);
            break;
        }
        case OPCODE_LT:
        {
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//LT\n");
#endif
            CallHLSLOpcodeFunc2(psContext, "HLSL_lt", psInst);
            break;
        }
        case OPCODE_IEQ:
        {
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//IEQ\n");
#endif
            CallHLSLOpcodeFunc2(psContext, "HLSL_ieq", psInst);
            break;
        }
        case OPCODE_FTOI:
        {
            // Rounding is always performed towards zero

            //Use int constructor - int(float). This drops the fractional part.
        }
        case OPCODE_MOVC:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MOVC\n");
            CallHLSLOpcodeFunc1(psContext, "HLSL_movc", psInst);
            break;
        }
		case OPCODE_LOG:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//LOG\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = log(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_RSQ:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//RSQ\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = inversesqrt(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_EXP:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//EXP\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = exp(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_SQRT:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//SQRT\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = sqrt(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_ROUND_PI:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//ROUND_PI\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = ceil(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_ROUND_NI:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//ROUND_NI\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = floor(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_ROUND_Z:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//ROUND_Z\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = trunc(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_ROUND_NE:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//ROUND_NE\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = roundEven(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_FRC:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//FRC\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = fract(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_MAX:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MAX\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = max(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_MIN:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//MIN\n");
            AddIndentation(psContext);
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, " = min(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_SAMPLE:
        {
            //dest, coords, tex, sampler
            AddIndentation(psContext);
            bcatcstr(glsl, "//SAMPLE\n");
            AddIndentation(psContext);//1=temp??
            TranslateOperand(psContext, &psInst->asOperands[1]);//??
            bcatcstr(glsl, " = texture2D(");

            TranslateOperand(psContext, &psInst->asOperands[3]);//resource
            bcatcstr(glsl, ", ");
            //Texture coord cannot be vec4
            //Determining if it is a vec3 for vec2 yet to be done.
            psInst->asOperands[2].aui32Swizzle[2] = 0xFFFFFFFF;
            psInst->asOperands[2].aui32Swizzle[3] = 0xFFFFFFFF;
            TranslateOperand(psContext, &psInst->asOperands[2]);//in
            bcatcstr(glsl, ");\n");
            break;
        }
        case OPCODE_SAMPLE_L:
        {
            //dest, coords, tex, sampler, lod
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//SAMPLE_L\n");
#endif
            AddIndentation(psContext);
            bcatcstr(glsl, "HLSL_sample_l");
            bcatcstr(glsl, "(");
            TranslateOperand(psContext, &psInst->asOperands[1]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[3]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[2]);
            bcatcstr(glsl, ", ");
            TranslateOperand(psContext, &psInst->asOperands[5]);
            bcatcstr(glsl, ");\n");
            break;
        }
		case OPCODE_RET:
		{
            AddIndentation(psContext);
            bcatcstr(glsl, "//RET\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "return;\n");
			break;
		}
        case OPCODE_CUT:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//CUT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EndPrimitive();\n");
			break;
        }
        case OPCODE_EMIT:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//EMIT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EmitVertex();\n");
			break;
        }
        case OPCODE_EMITTHENCUT:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//EMITTHENCUT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EmitVertex();\nEndPrimitive();\n");
			break;
        }

        case OPCODE_CUT_STREAM:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//CUT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EndStreamPrimitive(");
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ");\n");

			break;
        }
        case OPCODE_EMIT_STREAM:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//EMIT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EmitStreamVertex(");
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ");\n");
			break;
        }
        case OPCODE_EMITTHENCUT_STREAM:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//EMITTHENCUT\n");
            AddIndentation(psContext);
			bcatcstr(glsl, "EmitStreamVertex(");
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ");\n");
			bcatcstr(glsl, "EndStreamPrimitive(");
            TranslateOperand(psContext, &psInst->asOperands[0]);
            bcatcstr(glsl, ");\n");
			break;
        }
        case OPCODE_LOOP:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//LOOP\n");
            AddIndentation(psContext);
            bcatcstr(glsl, "while(true){\n");
            ++psContext->indent;
            break;
        }
        case OPCODE_ENDLOOP:
        {
            --psContext->indent;
            AddIndentation(psContext);
            bcatcstr(glsl, "//ENDLOOP\n");
            AddIndentation(psContext);
            bcatcstr(glsl, "}\n");
            break;
        }
        case OPCODE_BREAK:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//BREAK\n");
            AddIndentation(psContext);
            bcatcstr(glsl, "break;\n");
            break;
        }
        case OPCODE_BREAKC:
        {
#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//BREAKC\n");
#endif
            AddIndentation(psContext);
            if(psInst->eBooleanTestType == INSTRUCTION_TEST_ZERO)
            {
                bcatcstr(glsl, "if((");
                TranslateOperand(psContext, &psInst->asOperands[0]);
                bcatcstr(glsl, ")==0){break;}\n");
            }
            else
            {
                ASSERT(psInst->eBooleanTestType == INSTRUCTION_TEST_NONZERO);
                bcatcstr(glsl, "if((");
                TranslateOperand(psContext, &psInst->asOperands[0]);
                bcatcstr(glsl, ")!=0){break;}\n");
            }
            break;
        }
        case OPCODE_IF:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//IF\n");
            AddIndentation(psContext);
            if(psInst->eBooleanTestType == INSTRUCTION_TEST_ZERO)
            {
                bcatcstr(glsl, "if((");
                TranslateOperand(psContext, &psInst->asOperands[0]);
                bcatcstr(glsl, ")==0){\n");
            }
            else
            {
                ASSERT(psInst->eBooleanTestType == INSTRUCTION_TEST_NONZERO);
                bcatcstr(glsl, "if((");
                TranslateOperand(psContext, &psInst->asOperands[0]);
                bcatcstr(glsl, ")!=0){\n");
            }
            ++psContext->indent;
            break;
        }
        case OPCODE_ELSE:
        {
            --psContext->indent;
            AddIndentation(psContext);
            bcatcstr(glsl, "//ELSE\n");
            AddIndentation(psContext);
            bcatcstr(glsl, "} else {\n");
            psContext->indent++;
            break;
        }
        case OPCODE_ENDSWITCH:
        case OPCODE_ENDIF:
        {
            --psContext->indent;
            AddIndentation(psContext);
            bcatcstr(glsl, "//ENDIF\n");
            AddIndentation(psContext);
            bcatcstr(glsl, "}\n");
            break;
        }
        case OPCODE_CONTINUE:
        {
            bcatcstr(glsl, "continue;\n");
            break;
        }
        case OPCODE_DEFAULT:
        {
            bcatcstr(glsl, "default:\n");
            ++psContext->indent;
            break;
        }
        case OPCODE_NOP:
        {
            break;
        }
        case OPCODE_SYNC:
        {
            const uint32_t ui32SyncFlags = psInst->ui32SyncFlags;

#ifdef _DEBUG
            AddIndentation(psContext);
            bcatcstr(glsl, "//SYNC\n");
#endif

            AddIndentation(psContext);

            if(ui32SyncFlags & SYNC_THREADS_IN_GROUP)
            {
                bcatcstr(glsl, "groupMemoryBarrier();\n");
            }
            if(ui32SyncFlags & SYNC_THREAD_GROUP_SHARED_MEMORY)
            {
                bcatcstr(glsl, "memoryBarrierShared();\n");
            }
            if(ui32SyncFlags & (SYNC_UNORDERED_ACCESS_VIEW_MEMORY_GROUP|SYNC_UNORDERED_ACCESS_VIEW_MEMORY_GLOBAL))
            {
                bcatcstr(glsl, "memoryBarrier();\n");
            }
            break;
        }
        default:
        {
            AddIndentation(psContext);
            bcatcstr(glsl, "//Unknown opcode\n");
#ifdef _DEBUG
            printf("Unknown opcode (toGLSL).\n");
#endif
            break;
        }
    }
}