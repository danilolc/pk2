// https://github.com/mattiaisgro/loadgl
#pragma once

#include <GL/glcorearb.h>
//#include <GLES2/gl2.h>

namespace PGlFuncs {

int load_lib();
void free_lib();

}

// extern PFNGLACTIVESHADERPROGRAMPROC gl_ActiveShaderProgram;
extern PFNGLACTIVETEXTUREPROC gl_ActiveTexture;
extern PFNGLATTACHSHADERPROC gl_AttachShader;
// extern PFNGLBEGINCONDITIONALRENDERPROC gl_BeginConditionalRender;
// extern PFNGLBEGINQUERYPROC gl_BeginQuery;
// extern PFNGLBEGINQUERYINDEXEDPROC gl_BeginQueryIndexed;
// extern PFNGLBEGINTRANSFORMFEEDBACKPROC gl_BeginTransformFeedback;
extern PFNGLBINDATTRIBLOCATIONPROC gl_BindAttribLocation;
extern PFNGLBINDBUFFERPROC gl_BindBuffer;
// extern PFNGLBINDBUFFERBASEPROC gl_BindBufferBase;
// extern PFNGLBINDBUFFERRANGEPROC gl_BindBufferRange;
// extern PFNGLBINDBUFFERSBASEPROC gl_BindBuffersBase;
// extern PFNGLBINDBUFFERSRANGEPROC gl_BindBuffersRange;
extern PFNGLBINDFRAGDATALOCATIONPROC gl_BindFragDataLocation;
// extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC gl_BindFragDataLocationIndexed;
extern PFNGLBINDFRAMEBUFFERPROC gl_BindFramebuffer;
// extern PFNGLBINDIMAGETEXTUREPROC gl_BindImageTexture;
// extern PFNGLBINDIMAGETEXTURESPROC gl_BindImageTextures;
// extern PFNGLBINDPROGRAMPIPELINEPROC gl_BindProgramPipeline;
// extern PFNGLBINDRENDERBUFFERPROC gl_BindRenderbuffer;
// extern PFNGLBINDSAMPLERPROC gl_BindSampler;
// extern PFNGLBINDSAMPLERSPROC gl_BindSamplers;
extern PFNGLBINDTEXTUREPROC gl_BindTexture;
// extern PFNGLBINDTEXTUREUNITPROC gl_BindTextureUnit;
// extern PFNGLBINDTEXTURESPROC gl_BindTextures;
// extern PFNGLBINDTRANSFORMFEEDBACKPROC gl_BindTransformFeedback;
extern PFNGLBINDVERTEXARRAYPROC gl_BindVertexArray;
// extern PFNGLBINDVERTEXBUFFERPROC gl_BindVertexBuffer;
// extern PFNGLBINDVERTEXBUFFERSPROC gl_BindVertexBuffers;
// extern PFNGLBLENDCOLORPROC gl_BlendColor;
// extern PFNGLBLENDEQUATIONPROC gl_BlendEquation;
// extern PFNGLBLENDEQUATIONSEPARATEPROC gl_BlendEquationSeparate;
// extern PFNGLBLENDEQUATIONSEPARATEIPROC gl_BlendEquationSeparatei;
// extern PFNGLBLENDEQUATIONSEPARATEIARBPROC gl_BlendEquationSeparateiARB;
// extern PFNGLBLENDEQUATIONIPROC gl_BlendEquationi;
// extern PFNGLBLENDEQUATIONIARBPROC gl_BlendEquationiARB;
// extern PFNGLBLENDFUNCPROC gl_BlendFunc;
// extern PFNGLBLENDFUNCSEPARATEPROC gl_BlendFuncSeparate;
// extern PFNGLBLENDFUNCSEPARATEIPROC gl_BlendFuncSeparatei;
// extern PFNGLBLENDFUNCSEPARATEIARBPROC gl_BlendFuncSeparateiARB;
// extern PFNGLBLENDFUNCIPROC gl_BlendFunci;
// extern PFNGLBLENDFUNCIARBPROC gl_BlendFunciARB;
// extern PFNGLBLITFRAMEBUFFERPROC gl_BlitFramebuffer;
// extern PFNGLBLITNAMEDFRAMEBUFFERPROC gl_BlitNamedFramebuffer;
extern PFNGLBUFFERDATAPROC gl_BufferData;
// extern PFNGLBUFFERPAGECOMMITMENTARBPROC gl_BufferPageCommitmentARB;
// extern PFNGLBUFFERSTORAGEPROC gl_BufferStorage;
// extern PFNGLBUFFERSUBDATAPROC gl_BufferSubData;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC gl_CheckFramebufferStatus;
// extern PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC gl_CheckNamedFramebufferStatus;
// extern PFNGLCLAMPCOLORPROC gl_ClampColor;
extern PFNGLCLEARPROC gl_Clear;
// extern PFNGLCLEARBUFFERDATAPROC gl_ClearBufferData;
// extern PFNGLCLEARBUFFERSUBDATAPROC gl_ClearBufferSubData;
// extern PFNGLCLEARBUFFERFIPROC gl_ClearBufferfi;
// extern PFNGLCLEARBUFFERFVPROC gl_ClearBufferfv;
// extern PFNGLCLEARBUFFERIVPROC gl_ClearBufferiv;
// extern PFNGLCLEARBUFFERUIVPROC gl_ClearBufferuiv;
extern PFNGLCLEARCOLORPROC gl_ClearColor;
// extern PFNGLCLEARDEPTHPROC gl_ClearDepth;
// extern PFNGLCLEARDEPTHFPROC gl_ClearDepthf;
// extern PFNGLCLEARNAMEDBUFFERDATAPROC gl_ClearNamedBufferData;
// extern PFNGLCLEARNAMEDBUFFERSUBDATAPROC gl_ClearNamedBufferSubData;
// extern PFNGLCLEARNAMEDFRAMEBUFFERFIPROC gl_ClearNamedFramebufferfi;
// extern PFNGLCLEARNAMEDFRAMEBUFFERFVPROC gl_ClearNamedFramebufferfv;
// extern PFNGLCLEARNAMEDFRAMEBUFFERIVPROC gl_ClearNamedFramebufferiv;
// extern PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC gl_ClearNamedFramebufferuiv;
// extern PFNGLCLEARSTENCILPROC gl_ClearStencil;
// extern PFNGLCLEARTEXIMAGEPROC gl_ClearTexImage;
// extern PFNGLCLEARTEXSUBIMAGEPROC gl_ClearTexSubImage;
// extern PFNGLCLIENTWAITSYNCPROC gl_ClientWaitSync;
// extern PFNGLCLIPCONTROLPROC gl_ClipControl;
// extern PFNGLCOLORMASKPROC gl_ColorMask;
// extern PFNGLCOLORMASKIPROC gl_ColorMaski;
extern PFNGLCOMPILESHADERPROC gl_CompileShader;
// extern PFNGLCOMPILESHADERINCLUDEARBPROC gl_CompileShaderIncludeARB;
// extern PFNGLCOMPRESSEDTEXIMAGE1DPROC gl_CompressedTexImage1D;
// extern PFNGLCOMPRESSEDTEXIMAGE2DPROC gl_CompressedTexImage2D;
// extern PFNGLCOMPRESSEDTEXIMAGE3DPROC gl_CompressedTexImage3D;
// extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC gl_CompressedTexSubImage1D;
// extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC gl_CompressedTexSubImage2D;
// extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC gl_CompressedTexSubImage3D;
// extern PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC gl_CompressedTextureSubImage1D;
// extern PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC gl_CompressedTextureSubImage2D;
// extern PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC gl_CompressedTextureSubImage3D;
// extern PFNGLCOPYBUFFERSUBDATAPROC gl_CopyBufferSubData;
// extern PFNGLCOPYIMAGESUBDATAPROC gl_CopyImageSubData;
// extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC gl_CopyNamedBufferSubData;
// extern PFNGLCOPYTEXIMAGE1DPROC gl_CopyTexImage1D;
// extern PFNGLCOPYTEXIMAGE2DPROC gl_CopyTexImage2D;
// extern PFNGLCOPYTEXSUBIMAGE1DPROC gl_CopyTexSubImage1D;
// extern PFNGLCOPYTEXSUBIMAGE2DPROC gl_CopyTexSubImage2D;
// extern PFNGLCOPYTEXSUBIMAGE3DPROC gl_CopyTexSubImage3D;
// extern PFNGLCOPYTEXTURESUBIMAGE1DPROC gl_CopyTextureSubImage1D;
// extern PFNGLCOPYTEXTURESUBIMAGE2DPROC gl_CopyTextureSubImage2D;
// extern PFNGLCOPYTEXTURESUBIMAGE3DPROC gl_CopyTextureSubImage3D;
// extern PFNGLCREATEBUFFERSPROC gl_CreateBuffers;
// extern PFNGLCREATEFRAMEBUFFERSPROC gl_CreateFramebuffers;
extern PFNGLCREATEPROGRAMPROC gl_CreateProgram;
// extern PFNGLCREATEPROGRAMPIPELINESPROC gl_CreateProgramPipelines;
// extern PFNGLCREATEQUERIESPROC gl_CreateQueries;
// extern PFNGLCREATERENDERBUFFERSPROC gl_CreateRenderbuffers;
// extern PFNGLCREATESAMPLERSPROC gl_CreateSamplers;
extern PFNGLCREATESHADERPROC gl_CreateShader;
// extern PFNGLCREATESHADERPROGRAMVPROC gl_CreateShaderProgramv;
// extern PFNGLCREATESYNCFROMCLEVENTARBPROC gl_CreateSyncFromCLeventARB;
// extern PFNGLCREATETEXTURESPROC gl_CreateTextures;
// extern PFNGLCREATETRANSFORMFEEDBACKSPROC gl_CreateTransformFeedbacks;
// extern PFNGLCREATEVERTEXARRAYSPROC gl_CreateVertexArrays;
// extern PFNGLCULLFACEPROC gl_CullFace;
// extern PFNGLDEBUGMESSAGECALLBACKPROC gl_DebugMessageCallback;
// extern PFNGLDEBUGMESSAGECALLBACKARBPROC gl_DebugMessageCallbackARB;
// extern PFNGLDEBUGMESSAGECONTROLPROC gl_DebugMessageControl;
// extern PFNGLDEBUGMESSAGECONTROLARBPROC gl_DebugMessageControlARB;
// extern PFNGLDEBUGMESSAGEINSERTPROC gl_DebugMessageInsert;
// extern PFNGLDEBUGMESSAGEINSERTARBPROC gl_DebugMessageInsertARB;
extern PFNGLDELETEBUFFERSPROC gl_DeleteBuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC gl_DeleteFramebuffers;
// extern PFNGLDELETENAMEDSTRINGARBPROC gl_DeleteNamedStringARB;
extern PFNGLDELETEPROGRAMPROC gl_DeleteProgram;
// extern PFNGLDELETEPROGRAMPIPELINESPROC gl_DeleteProgramPipelines;
// extern PFNGLDELETEQUERIESPROC gl_DeleteQueries;
// extern PFNGLDELETERENDERBUFFERSPROC gl_DeleteRenderbuffers;
// extern PFNGLDELETESAMPLERSPROC gl_DeleteSamplers;
extern PFNGLDELETESHADERPROC gl_DeleteShader;
// extern PFNGLDELETESYNCPROC gl_DeleteSync;
extern PFNGLDELETETEXTURESPROC gl_DeleteTextures;
// extern PFNGLDELETETRANSFORMFEEDBACKSPROC gl_DeleteTransformFeedbacks;
extern PFNGLDELETEVERTEXARRAYSPROC gl_DeleteVertexArrays;
// extern PFNGLDEPTHFUNCPROC gl_DepthFunc;
// extern PFNGLDEPTHMASKPROC gl_DepthMask;
// extern PFNGLDEPTHRANGEPROC gl_DepthRange;
// extern PFNGLDEPTHRANGEARRAYVPROC gl_DepthRangeArrayv;
// extern PFNGLDEPTHRANGEINDEXEDPROC gl_DepthRangeIndexed;
// extern PFNGLDEPTHRANGEFPROC gl_DepthRangef;
// extern PFNGLDETACHSHADERPROC gl_DetachShader;
extern PFNGLDISABLEPROC gl_Disable;
// extern PFNGLDISABLEVERTEXARRAYATTRIBPROC gl_DisableVertexArrayAttrib;
// extern PFNGLDISABLEVERTEXATTRIBARRAYPROC gl_DisableVertexAttribArray;
// extern PFNGLDISABLEIPROC gl_Disablei;
// extern PFNGLDISPATCHCOMPUTEPROC gl_DispatchCompute;
// extern PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC gl_DispatchComputeGroupSizeARB;
// extern PFNGLDISPATCHCOMPUTEINDIRECTPROC gl_DispatchComputeIndirect;
extern PFNGLDRAWARRAYSPROC gl_DrawArrays;
// extern PFNGLDRAWARRAYSINDIRECTPROC gl_DrawArraysIndirect;
// extern PFNGLDRAWARRAYSINSTANCEDPROC gl_DrawArraysInstanced;
// extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC gl_DrawArraysInstancedBaseInstance;
// extern PFNGLDRAWBUFFERPROC gl_DrawBuffer;
extern PFNGLDRAWBUFFERSPROC gl_DrawBuffers;
// extern PFNGLDRAWELEMENTSPROC gl_DrawElements;
// extern PFNGLDRAWELEMENTSBASEVERTEXPROC gl_DrawElementsBaseVertex;
// extern PFNGLDRAWELEMENTSINDIRECTPROC gl_DrawElementsIndirect;
// extern PFNGLDRAWELEMENTSINSTANCEDPROC gl_DrawElementsInstanced;
// extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC gl_DrawElementsInstancedBaseInstance;
// extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC gl_DrawElementsInstancedBaseVertex;
// extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC gl_DrawElementsInstancedBaseVertexBaseInstance;
// extern PFNGLDRAWRANGEELEMENTSPROC gl_DrawRangeElements;
// extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC gl_DrawRangeElementsBaseVertex;
// extern PFNGLDRAWTRANSFORMFEEDBACKPROC gl_DrawTransformFeedback;
// extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC gl_DrawTransformFeedbackInstanced;
// extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC gl_DrawTransformFeedbackStream;
// extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC gl_DrawTransformFeedbackStreamInstanced;
extern PFNGLENABLEPROC gl_Enable;
// extern PFNGLENABLEVERTEXARRAYATTRIBPROC gl_EnableVertexArrayAttrib;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC gl_EnableVertexAttribArray;
// extern PFNGLENABLEIPROC gl_Enablei;
// extern PFNGLENDCONDITIONALRENDERPROC gl_EndConditionalRender;
// extern PFNGLENDQUERYPROC gl_EndQuery;
// extern PFNGLENDQUERYINDEXEDPROC gl_EndQueryIndexed;
// extern PFNGLENDTRANSFORMFEEDBACKPROC gl_EndTransformFeedback;
// extern PFNGLFENCESYNCPROC gl_FenceSync;
// extern PFNGLFINISHPROC gl_Finish;
// extern PFNGLFLUSHPROC gl_Flush;
// extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC gl_FlushMappedBufferRange;
// extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC gl_FlushMappedNamedBufferRange;
// extern PFNGLFRAMEBUFFERPARAMETERIPROC gl_FramebufferParameteri;
// extern PFNGLFRAMEBUFFERRENDERBUFFERPROC gl_FramebufferRenderbuffer;
// extern PFNGLFRAMEBUFFERTEXTUREPROC gl_FramebufferTexture;
// extern PFNGLFRAMEBUFFERTEXTURE1DPROC gl_FramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC gl_FramebufferTexture2D;
// extern PFNGLFRAMEBUFFERTEXTURE3DPROC gl_FramebufferTexture3D;
// extern PFNGLFRAMEBUFFERTEXTURELAYERPROC gl_FramebufferTextureLayer;
// extern PFNGLFRONTFACEPROC gl_FrontFace;
extern PFNGLGENBUFFERSPROC gl_GenBuffers;
extern PFNGLGENFRAMEBUFFERSPROC gl_GenFramebuffers;
// extern PFNGLGENPROGRAMPIPELINESPROC gl_GenProgramPipelines;
// extern PFNGLGENQUERIESPROC gl_GenQueries;
// extern PFNGLGENRENDERBUFFERSPROC gl_GenRenderbuffers;
// extern PFNGLGENSAMPLERSPROC gl_GenSamplers;
extern PFNGLGENTEXTURESPROC gl_GenTextures;
// extern PFNGLGENTRANSFORMFEEDBACKSPROC gl_GenTransformFeedbacks;
extern PFNGLGENVERTEXARRAYSPROC gl_GenVertexArrays;
// extern PFNGLGENERATEMIPMAPPROC gl_GenerateMipmap;
// extern PFNGLGENERATETEXTUREMIPMAPPROC gl_GenerateTextureMipmap;
// extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC gl_GetActiveAtomicCounterBufferiv;
// extern PFNGLGETACTIVEATTRIBPROC gl_GetActiveAttrib;
// extern PFNGLGETACTIVESUBROUTINENAMEPROC gl_GetActiveSubroutineName;
// extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC gl_GetActiveSubroutineUniformName;
// extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC gl_GetActiveSubroutineUniformiv;
// extern PFNGLGETACTIVEUNIFORMPROC gl_GetActiveUniform;
// extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC gl_GetActiveUniformBlockName;
// extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC gl_GetActiveUniformBlockiv;
// extern PFNGLGETACTIVEUNIFORMNAMEPROC gl_GetActiveUniformName;
// extern PFNGLGETACTIVEUNIFORMSIVPROC gl_GetActiveUniformsiv;
// extern PFNGLGETATTACHEDSHADERSPROC gl_GetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC gl_GetAttribLocation;
// extern PFNGLGETBOOLEANI_VPROC gl_GetBooleani_v;
// extern PFNGLGETBOOLEANVPROC gl_GetBooleanv;
// extern PFNGLGETBUFFERPARAMETERI64VPROC gl_GetBufferParameteri64v;
// extern PFNGLGETBUFFERPARAMETERIVPROC gl_GetBufferParameteriv;
// extern PFNGLGETBUFFERPOINTERVPROC gl_GetBufferPointerv;
// extern PFNGLGETBUFFERSUBDATAPROC gl_GetBufferSubData;
// extern PFNGLGETCOMPRESSEDTEXIMAGEPROC gl_GetCompressedTexImage;
// extern PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC gl_GetCompressedTextureImage;
// extern PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC gl_GetCompressedTextureSubImage;
// extern PFNGLGETDEBUGMESSAGELOGPROC gl_GetDebugMessageLog;
// extern PFNGLGETDEBUGMESSAGELOGARBPROC gl_GetDebugMessageLogARB;
// extern PFNGLGETDOUBLEI_VPROC gl_GetDoublei_v;
// extern PFNGLGETDOUBLEVPROC gl_GetDoublev;
// extern PFNGLGETERRORPROC gl_GetError;
// extern PFNGLGETFLOATI_VPROC gl_GetFloati_v;
// extern PFNGLGETFLOATVPROC gl_GetFloatv;
// extern PFNGLGETFRAGDATAINDEXPROC gl_GetFragDataIndex;
// extern PFNGLGETFRAGDATALOCATIONPROC gl_GetFragDataLocation;
// extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC gl_GetFramebufferAttachmentParameteriv;
// extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC gl_GetFramebufferParameteriv;
// extern PFNGLGETGRAPHICSRESETSTATUSPROC gl_GetGraphicsResetStatus;
// extern PFNGLGETGRAPHICSRESETSTATUSARBPROC gl_GetGraphicsResetStatusARB;
// extern PFNGLGETIMAGEHANDLEARBPROC gl_GetImageHandleARB;
// extern PFNGLGETINTEGER64I_VPROC gl_GetInteger64i_v;
// extern PFNGLGETINTEGER64VPROC gl_GetInteger64v;
// extern PFNGLGETINTEGERI_VPROC gl_GetIntegeri_v;
extern PFNGLGETINTEGERVPROC gl_GetIntegerv;
// extern PFNGLGETINTERNALFORMATI64VPROC gl_GetInternalformati64v;
// extern PFNGLGETINTERNALFORMATIVPROC gl_GetInternalformativ;
// extern PFNGLGETMULTISAMPLEFVPROC gl_GetMultisamplefv;
// extern PFNGLGETNAMEDBUFFERPARAMETERI64VPROC gl_GetNamedBufferParameteri64v;
// extern PFNGLGETNAMEDBUFFERPARAMETERIVPROC gl_GetNamedBufferParameteriv;
// extern PFNGLGETNAMEDBUFFERPOINTERVPROC gl_GetNamedBufferPointerv;
// extern PFNGLGETNAMEDBUFFERSUBDATAPROC gl_GetNamedBufferSubData;
// extern PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC gl_GetNamedFramebufferAttachmentParameteriv;
// extern PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC gl_GetNamedFramebufferParameteriv;
// extern PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC gl_GetNamedRenderbufferParameteriv;
// extern PFNGLGETNAMEDSTRINGARBPROC gl_GetNamedStringARB;
// extern PFNGLGETNAMEDSTRINGIVARBPROC gl_GetNamedStringivARB;
// extern PFNGLGETOBJECTLABELPROC gl_GetObjectLabel;
// extern PFNGLGETOBJECTPTRLABELPROC gl_GetObjectPtrLabel;
// extern PFNGLGETPOINTERVPROC gl_GetPointerv;
// extern PFNGLGETPROGRAMBINARYPROC gl_GetProgramBinary;
extern PFNGLGETPROGRAMINFOLOGPROC gl_GetProgramInfoLog;
// extern PFNGLGETPROGRAMINTERFACEIVPROC gl_GetProgramInterfaceiv;
// extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC gl_GetProgramPipelineInfoLog;
// extern PFNGLGETPROGRAMPIPELINEIVPROC gl_GetProgramPipelineiv;
// extern PFNGLGETPROGRAMRESOURCEINDEXPROC gl_GetProgramResourceIndex;
// extern PFNGLGETPROGRAMRESOURCELOCATIONPROC gl_GetProgramResourceLocation;
// extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC gl_GetProgramResourceLocationIndex;
// extern PFNGLGETPROGRAMRESOURCENAMEPROC gl_GetProgramResourceName;
// extern PFNGLGETPROGRAMRESOURCEIVPROC gl_GetProgramResourceiv;
// extern PFNGLGETPROGRAMSTAGEIVPROC gl_GetProgramStageiv;
extern PFNGLGETPROGRAMIVPROC gl_GetProgramiv;
// extern PFNGLGETQUERYBUFFEROBJECTI64VPROC gl_GetQueryBufferObjecti64v;
// extern PFNGLGETQUERYBUFFEROBJECTIVPROC gl_GetQueryBufferObjectiv;
// extern PFNGLGETQUERYBUFFEROBJECTUI64VPROC gl_GetQueryBufferObjectui64v;
// extern PFNGLGETQUERYBUFFEROBJECTUIVPROC gl_GetQueryBufferObjectuiv;
// extern PFNGLGETQUERYINDEXEDIVPROC gl_GetQueryIndexediv;
// extern PFNGLGETQUERYOBJECTI64VPROC gl_GetQueryObjecti64v;
// extern PFNGLGETQUERYOBJECTIVPROC gl_GetQueryObjectiv;
// extern PFNGLGETQUERYOBJECTUI64VPROC gl_GetQueryObjectui64v;
// extern PFNGLGETQUERYOBJECTUIVPROC gl_GetQueryObjectuiv;
// extern PFNGLGETQUERYIVPROC gl_GetQueryiv;
// extern PFNGLGETRENDERBUFFERPARAMETERIVPROC gl_GetRenderbufferParameteriv;
// extern PFNGLGETSAMPLERPARAMETERIIVPROC gl_GetSamplerParameterIiv;
// extern PFNGLGETSAMPLERPARAMETERIUIVPROC gl_GetSamplerParameterIuiv;
// extern PFNGLGETSAMPLERPARAMETERFVPROC gl_GetSamplerParameterfv;
// extern PFNGLGETSAMPLERPARAMETERIVPROC gl_GetSamplerParameteriv;
extern PFNGLGETSHADERINFOLOGPROC gl_GetShaderInfoLog;
// extern PFNGLGETSHADERPRECISIONFORMATPROC gl_GetShaderPrecisionFormat;
// extern PFNGLGETSHADERSOURCEPROC gl_GetShaderSource;
extern PFNGLGETSHADERIVPROC gl_GetShaderiv;
extern PFNGLGETSTRINGPROC gl_GetString;
// extern PFNGLGETSTRINGIPROC gl_GetStringi;
// extern PFNGLGETSUBROUTINEINDEXPROC gl_GetSubroutineIndex;
// extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC gl_GetSubroutineUniformLocation;
// extern PFNGLGETSYNCIVPROC gl_GetSynciv;
// extern PFNGLGETTEXIMAGEPROC gl_GetTexImage;
// extern PFNGLGETTEXLEVELPARAMETERFVPROC gl_GetTexLevelParameterfv;
// extern PFNGLGETTEXLEVELPARAMETERIVPROC gl_GetTexLevelParameteriv;
// extern PFNGLGETTEXPARAMETERIIVPROC gl_GetTexParameterIiv;
// extern PFNGLGETTEXPARAMETERIUIVPROC gl_GetTexParameterIuiv;
// extern PFNGLGETTEXPARAMETERFVPROC gl_GetTexParameterfv;
// extern PFNGLGETTEXPARAMETERIVPROC gl_GetTexParameteriv;
// extern PFNGLGETTEXTUREHANDLEARBPROC gl_GetTextureHandleARB;
// extern PFNGLGETTEXTUREIMAGEPROC gl_GetTextureImage;
// extern PFNGLGETTEXTURELEVELPARAMETERFVPROC gl_GetTextureLevelParameterfv;
// extern PFNGLGETTEXTURELEVELPARAMETERIVPROC gl_GetTextureLevelParameteriv;
// extern PFNGLGETTEXTUREPARAMETERIIVPROC gl_GetTextureParameterIiv;
// extern PFNGLGETTEXTUREPARAMETERIUIVPROC gl_GetTextureParameterIuiv;
// extern PFNGLGETTEXTUREPARAMETERFVPROC gl_GetTextureParameterfv;
// extern PFNGLGETTEXTUREPARAMETERIVPROC gl_GetTextureParameteriv;
// extern PFNGLGETTEXTURESAMPLERHANDLEARBPROC gl_GetTextureSamplerHandleARB;
// extern PFNGLGETTEXTURESUBIMAGEPROC gl_GetTextureSubImage;
// extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC gl_GetTransformFeedbackVarying;
// extern PFNGLGETTRANSFORMFEEDBACKI64_VPROC gl_GetTransformFeedbacki64_v;
// extern PFNGLGETTRANSFORMFEEDBACKI_VPROC gl_GetTransformFeedbacki_v;
// extern PFNGLGETTRANSFORMFEEDBACKIVPROC gl_GetTransformFeedbackiv;
// extern PFNGLGETUNIFORMBLOCKINDEXPROC gl_GetUniformBlockIndex;
// extern PFNGLGETUNIFORMINDICESPROC gl_GetUniformIndices;
extern PFNGLGETUNIFORMLOCATIONPROC gl_GetUniformLocation;
// extern PFNGLGETUNIFORMSUBROUTINEUIVPROC gl_GetUniformSubroutineuiv;
// extern PFNGLGETUNIFORMDVPROC gl_GetUniformdv;
// extern PFNGLGETUNIFORMFVPROC gl_GetUniformfv;
// extern PFNGLGETUNIFORMIVPROC gl_GetUniformiv;
// extern PFNGLGETUNIFORMUIVPROC gl_GetUniformuiv;
// extern PFNGLGETVERTEXARRAYINDEXED64IVPROC gl_GetVertexArrayIndexed64iv;
// extern PFNGLGETVERTEXARRAYINDEXEDIVPROC gl_GetVertexArrayIndexediv;
// extern PFNGLGETVERTEXARRAYIVPROC gl_GetVertexArrayiv;
// extern PFNGLGETVERTEXATTRIBIIVPROC gl_GetVertexAttribIiv;
// extern PFNGLGETVERTEXATTRIBIUIVPROC gl_GetVertexAttribIuiv;
// extern PFNGLGETVERTEXATTRIBLDVPROC gl_GetVertexAttribLdv;
// extern PFNGLGETVERTEXATTRIBLUI64VARBPROC gl_GetVertexAttribLui64vARB;
// extern PFNGLGETVERTEXATTRIBPOINTERVPROC gl_GetVertexAttribPointerv;
// extern PFNGLGETVERTEXATTRIBDVPROC gl_GetVertexAttribdv;
// extern PFNGLGETVERTEXATTRIBFVPROC gl_GetVertexAttribfv;
// extern PFNGLGETVERTEXATTRIBIVPROC gl_GetVertexAttribiv;
// extern PFNGLGETNCOMPRESSEDTEXIMAGEPROC gl_GetnCompressedTexImage;
// extern PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC gl_GetnCompressedTexImageARB;
// extern PFNGLGETNTEXIMAGEPROC gl_GetnTexImage;
// extern PFNGLGETNTEXIMAGEARBPROC gl_GetnTexImageARB;
// extern PFNGLGETNUNIFORMDVPROC gl_GetnUniformdv;
// extern PFNGLGETNUNIFORMDVARBPROC gl_GetnUniformdvARB;
// extern PFNGLGETNUNIFORMFVPROC gl_GetnUniformfv;
// extern PFNGLGETNUNIFORMFVARBPROC gl_GetnUniformfvARB;
// extern PFNGLGETNUNIFORMIVPROC gl_GetnUniformiv;
// extern PFNGLGETNUNIFORMIVARBPROC gl_GetnUniformivARB;
// extern PFNGLGETNUNIFORMUIVPROC gl_GetnUniformuiv;
// extern PFNGLGETNUNIFORMUIVARBPROC gl_GetnUniformuivARB;
// extern PFNGLHINTPROC gl_Hint;
// extern PFNGLINVALIDATEBUFFERDATAPROC gl_InvalidateBufferData;
// extern PFNGLINVALIDATEBUFFERSUBDATAPROC gl_InvalidateBufferSubData;
// extern PFNGLINVALIDATEFRAMEBUFFERPROC gl_InvalidateFramebuffer;
// extern PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC gl_InvalidateNamedFramebufferData;
// extern PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC gl_InvalidateNamedFramebufferSubData;
// extern PFNGLINVALIDATESUBFRAMEBUFFERPROC gl_InvalidateSubFramebuffer;
// extern PFNGLINVALIDATETEXIMAGEPROC gl_InvalidateTexImage;
// extern PFNGLINVALIDATETEXSUBIMAGEPROC gl_InvalidateTexSubImage;
// extern PFNGLISBUFFERPROC gl_IsBuffer;
// extern PFNGLISENABLEDPROC gl_IsEnabled;
// extern PFNGLISENABLEDIPROC gl_IsEnabledi;
// extern PFNGLISFRAMEBUFFERPROC gl_IsFramebuffer;
// extern PFNGLISIMAGEHANDLERESIDENTARBPROC gl_IsImageHandleResidentARB;
// extern PFNGLISNAMEDSTRINGARBPROC gl_IsNamedStringARB;
// extern PFNGLISPROGRAMPROC gl_IsProgram;
// extern PFNGLISPROGRAMPIPELINEPROC gl_IsProgramPipeline;
// extern PFNGLISQUERYPROC gl_IsQuery;
// extern PFNGLISRENDERBUFFERPROC gl_IsRenderbuffer;
// extern PFNGLISSAMPLERPROC gl_IsSampler;
// extern PFNGLISSHADERPROC gl_IsShader;
// extern PFNGLISSYNCPROC gl_IsSync;
// extern PFNGLISTEXTUREPROC gl_IsTexture;
// extern PFNGLISTEXTUREHANDLERESIDENTARBPROC gl_IsTextureHandleResidentARB;
// extern PFNGLISTRANSFORMFEEDBACKPROC gl_IsTransformFeedback;
// extern PFNGLISVERTEXARRAYPROC gl_IsVertexArray;
// extern PFNGLLINEWIDTHPROC gl_LineWidth;
extern PFNGLLINKPROGRAMPROC gl_LinkProgram;
// extern PFNGLLOGICOPPROC gl_LogicOp;
// extern PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC gl_MakeImageHandleNonResidentARB;
// extern PFNGLMAKEIMAGEHANDLERESIDENTARBPROC gl_MakeImageHandleResidentARB;
// extern PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC gl_MakeTextureHandleNonResidentARB;
// extern PFNGLMAKETEXTUREHANDLERESIDENTARBPROC gl_MakeTextureHandleResidentARB;
// extern PFNGLMAPBUFFERPROC gl_MapBuffer;
// extern PFNGLMAPBUFFERRANGEPROC gl_MapBufferRange;
// extern PFNGLMAPNAMEDBUFFERPROC gl_MapNamedBuffer;
// extern PFNGLMAPNAMEDBUFFERRANGEPROC gl_MapNamedBufferRange;
// extern PFNGLMEMORYBARRIERPROC gl_MemoryBarrier;
// extern PFNGLMEMORYBARRIERBYREGIONPROC gl_MemoryBarrierByRegion;
// extern PFNGLMINSAMPLESHADINGPROC gl_MinSampleShading;
// extern PFNGLMINSAMPLESHADINGARBPROC gl_MinSampleShadingARB;
// extern PFNGLMULTIDRAWARRAYSPROC gl_MultiDrawArrays;
// extern PFNGLMULTIDRAWARRAYSINDIRECTPROC gl_MultiDrawArraysIndirect;
// extern PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC gl_MultiDrawArraysIndirectCountARB;
// extern PFNGLMULTIDRAWELEMENTSPROC gl_MultiDrawElements;
// extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC gl_MultiDrawElementsBaseVertex;
// extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC gl_MultiDrawElementsIndirect;
// extern PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC gl_MultiDrawElementsIndirectCountARB;
// extern PFNGLNAMEDBUFFERDATAPROC gl_NamedBufferData;
// extern PFNGLNAMEDBUFFERPAGECOMMITMENTARBPROC gl_NamedBufferPageCommitmentARB;
// extern PFNGLNAMEDBUFFERPAGECOMMITMENTEXTPROC gl_NamedBufferPageCommitmentEXT;
// extern PFNGLNAMEDBUFFERSTORAGEPROC gl_NamedBufferStorage;
// extern PFNGLNAMEDBUFFERSUBDATAPROC gl_NamedBufferSubData;
// extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC gl_NamedFramebufferDrawBuffer;
// extern PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC gl_NamedFramebufferDrawBuffers;
// extern PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC gl_NamedFramebufferParameteri;
// extern PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC gl_NamedFramebufferReadBuffer;
// extern PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC gl_NamedFramebufferRenderbuffer;
// extern PFNGLNAMEDFRAMEBUFFERTEXTUREPROC gl_NamedFramebufferTexture;
// extern PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC gl_NamedFramebufferTextureLayer;
// extern PFNGLNAMEDRENDERBUFFERSTORAGEPROC gl_NamedRenderbufferStorage;
// extern PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC gl_NamedRenderbufferStorageMultisample;
// extern PFNGLNAMEDSTRINGARBPROC gl_NamedStringARB;
// extern PFNGLOBJECTLABELPROC gl_ObjectLabel;
// extern PFNGLOBJECTPTRLABELPROC gl_ObjectPtrLabel;
// extern PFNGLPATCHPARAMETERFVPROC gl_PatchParameterfv;
// extern PFNGLPATCHPARAMETERIPROC gl_PatchParameteri;
// extern PFNGLPAUSETRANSFORMFEEDBACKPROC gl_PauseTransformFeedback;
// extern PFNGLPIXELSTOREFPROC gl_PixelStoref;
// extern PFNGLPIXELSTOREIPROC gl_PixelStorei;
// extern PFNGLPOINTPARAMETERFPROC gl_PointParameterf;
// extern PFNGLPOINTPARAMETERFVPROC gl_PointParameterfv;
// extern PFNGLPOINTPARAMETERIPROC gl_PointParameteri;
// extern PFNGLPOINTPARAMETERIVPROC gl_PointParameteriv;
// extern PFNGLPOINTSIZEPROC gl_PointSize;
// extern PFNGLPOLYGONMODEPROC gl_PolygonMode;
// extern PFNGLPOLYGONOFFSETPROC gl_PolygonOffset;
// extern PFNGLPOPDEBUGGROUPPROC gl_PopDebugGroup;
// extern PFNGLPRIMITIVERESTARTINDEXPROC gl_PrimitiveRestartIndex;
// extern PFNGLPROGRAMBINARYPROC gl_ProgramBinary;
// extern PFNGLPROGRAMPARAMETERIPROC gl_ProgramParameteri;
// extern PFNGLPROGRAMUNIFORM1DPROC gl_ProgramUniform1d;
// extern PFNGLPROGRAMUNIFORM1DVPROC gl_ProgramUniform1dv;
// extern PFNGLPROGRAMUNIFORM1FPROC gl_ProgramUniform1f;
// extern PFNGLPROGRAMUNIFORM1FVPROC gl_ProgramUniform1fv;
// extern PFNGLPROGRAMUNIFORM1IPROC gl_ProgramUniform1i;
// extern PFNGLPROGRAMUNIFORM1IVPROC gl_ProgramUniform1iv;
// extern PFNGLPROGRAMUNIFORM1UIPROC gl_ProgramUniform1ui;
// extern PFNGLPROGRAMUNIFORM1UIVPROC gl_ProgramUniform1uiv;
// extern PFNGLPROGRAMUNIFORM2DPROC gl_ProgramUniform2d;
// extern PFNGLPROGRAMUNIFORM2DVPROC gl_ProgramUniform2dv;
// extern PFNGLPROGRAMUNIFORM2FPROC gl_ProgramUniform2f;
// extern PFNGLPROGRAMUNIFORM2FVPROC gl_ProgramUniform2fv;
// extern PFNGLPROGRAMUNIFORM2IPROC gl_ProgramUniform2i;
// extern PFNGLPROGRAMUNIFORM2IVPROC gl_ProgramUniform2iv;
// extern PFNGLPROGRAMUNIFORM2UIPROC gl_ProgramUniform2ui;
// extern PFNGLPROGRAMUNIFORM2UIVPROC gl_ProgramUniform2uiv;
// extern PFNGLPROGRAMUNIFORM3DPROC gl_ProgramUniform3d;
// extern PFNGLPROGRAMUNIFORM3DVPROC gl_ProgramUniform3dv;
// extern PFNGLPROGRAMUNIFORM3FPROC gl_ProgramUniform3f;
// extern PFNGLPROGRAMUNIFORM3FVPROC gl_ProgramUniform3fv;
// extern PFNGLPROGRAMUNIFORM3IPROC gl_ProgramUniform3i;
// extern PFNGLPROGRAMUNIFORM3IVPROC gl_ProgramUniform3iv;
// extern PFNGLPROGRAMUNIFORM3UIPROC gl_ProgramUniform3ui;
// extern PFNGLPROGRAMUNIFORM3UIVPROC gl_ProgramUniform3uiv;
// extern PFNGLPROGRAMUNIFORM4DPROC gl_ProgramUniform4d;
// extern PFNGLPROGRAMUNIFORM4DVPROC gl_ProgramUniform4dv;
// extern PFNGLPROGRAMUNIFORM4FPROC gl_ProgramUniform4f;
// extern PFNGLPROGRAMUNIFORM4FVPROC gl_ProgramUniform4fv;
// extern PFNGLPROGRAMUNIFORM4IPROC gl_ProgramUniform4i;
// extern PFNGLPROGRAMUNIFORM4IVPROC gl_ProgramUniform4iv;
// extern PFNGLPROGRAMUNIFORM4UIPROC gl_ProgramUniform4ui;
// extern PFNGLPROGRAMUNIFORM4UIVPROC gl_ProgramUniform4uiv;
// extern PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC gl_ProgramUniformHandleui64ARB;
// extern PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC gl_ProgramUniformHandleui64vARB;
// extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC gl_ProgramUniformMatrix2dv;
// extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC gl_ProgramUniformMatrix2fv;
// extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC gl_ProgramUniformMatrix2x3dv;
// extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC gl_ProgramUniformMatrix2x3fv;
// extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC gl_ProgramUniformMatrix2x4dv;
// extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC gl_ProgramUniformMatrix2x4fv;
// extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC gl_ProgramUniformMatrix3dv;
// extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC gl_ProgramUniformMatrix3fv;
// extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC gl_ProgramUniformMatrix3x2dv;
// extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC gl_ProgramUniformMatrix3x2fv;
// extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC gl_ProgramUniformMatrix3x4dv;
// extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC gl_ProgramUniformMatrix3x4fv;
// extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC gl_ProgramUniformMatrix4dv;
// extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC gl_ProgramUniformMatrix4fv;
// extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC gl_ProgramUniformMatrix4x2dv;
// extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC gl_ProgramUniformMatrix4x2fv;
// extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC gl_ProgramUniformMatrix4x3dv;
// extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC gl_ProgramUniformMatrix4x3fv;
// extern PFNGLPROVOKINGVERTEXPROC gl_ProvokingVertex;
// extern PFNGLPUSHDEBUGGROUPPROC gl_PushDebugGroup;
// extern PFNGLQUERYCOUNTERPROC gl_QueryCounter;
// extern PFNGLREADBUFFERPROC gl_ReadBuffer;
// extern PFNGLREADPIXELSPROC gl_ReadPixels;
// extern PFNGLREADNPIXELSPROC gl_ReadnPixels;
// extern PFNGLREADNPIXELSARBPROC gl_ReadnPixelsARB;
// extern PFNGLRELEASESHADERCOMPILERPROC gl_ReleaseShaderCompiler;
// extern PFNGLRENDERBUFFERSTORAGEPROC gl_RenderbufferStorage;
// extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC gl_RenderbufferStorageMultisample;
// extern PFNGLRESUMETRANSFORMFEEDBACKPROC gl_ResumeTransformFeedback;
// extern PFNGLSAMPLECOVERAGEPROC gl_SampleCoverage;
// extern PFNGLSAMPLEMASKIPROC gl_SampleMaski;
// extern PFNGLSAMPLERPARAMETERIIVPROC gl_SamplerParameterIiv;
// extern PFNGLSAMPLERPARAMETERIUIVPROC gl_SamplerParameterIuiv;
// extern PFNGLSAMPLERPARAMETERFPROC gl_SamplerParameterf;
// extern PFNGLSAMPLERPARAMETERFVPROC gl_SamplerParameterfv;
// extern PFNGLSAMPLERPARAMETERIPROC gl_SamplerParameteri;
// extern PFNGLSAMPLERPARAMETERIVPROC gl_SamplerParameteriv;
// extern PFNGLSCISSORPROC gl_Scissor;
// extern PFNGLSCISSORARRAYVPROC gl_ScissorArrayv;
// extern PFNGLSCISSORINDEXEDPROC gl_ScissorIndexed;
// extern PFNGLSCISSORINDEXEDVPROC gl_ScissorIndexedv;
// extern PFNGLSHADERBINARYPROC gl_ShaderBinary;
extern PFNGLSHADERSOURCEPROC gl_ShaderSource;
// extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC gl_ShaderStorageBlockBinding;
// extern PFNGLSTENCILFUNCPROC gl_StencilFunc;
// extern PFNGLSTENCILFUNCSEPARATEPROC gl_StencilFuncSeparate;
// extern PFNGLSTENCILMASKPROC gl_StencilMask;
// extern PFNGLSTENCILMASKSEPARATEPROC gl_StencilMaskSeparate;
// extern PFNGLSTENCILOPPROC gl_StencilOp;
// extern PFNGLSTENCILOPSEPARATEPROC gl_StencilOpSeparate;
// extern PFNGLTEXBUFFERPROC gl_TexBuffer;
// extern PFNGLTEXBUFFERRANGEPROC gl_TexBufferRange;
extern PFNGLTEXIMAGE1DPROC gl_TexImage1D;
extern PFNGLTEXIMAGE2DPROC gl_TexImage2D;
// extern PFNGLTEXIMAGE2DMULTISAMPLEPROC gl_TexImage2DMultisample;
// extern PFNGLTEXIMAGE3DPROC gl_TexImage3D;
// extern PFNGLTEXIMAGE3DMULTISAMPLEPROC gl_TexImage3DMultisample;
// extern PFNGLTEXPAGECOMMITMENTARBPROC gl_TexPageCommitmentARB;
// extern PFNGLTEXPARAMETERIIVPROC gl_TexParameterIiv;
// extern PFNGLTEXPARAMETERIUIVPROC gl_TexParameterIuiv;
// extern PFNGLTEXPARAMETERFPROC gl_TexParameterf;
// extern PFNGLTEXPARAMETERFVPROC gl_TexParameterfv;
extern PFNGLTEXPARAMETERIPROC gl_TexParameteri;
// extern PFNGLTEXPARAMETERIVPROC gl_TexParameteriv;
// extern PFNGLTEXSTORAGE1DPROC gl_TexStorage1D;
// extern PFNGLTEXSTORAGE2DPROC gl_TexStorage2D;
// extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC gl_TexStorage2DMultisample;
// extern PFNGLTEXSTORAGE3DPROC gl_TexStorage3D;
// extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC gl_TexStorage3DMultisample;
// extern PFNGLTEXSUBIMAGE1DPROC gl_TexSubImage1D;
// extern PFNGLTEXSUBIMAGE2DPROC gl_TexSubImage2D;
// extern PFNGLTEXSUBIMAGE3DPROC gl_TexSubImage3D;
// extern PFNGLTEXTUREBARRIERPROC gl_TextureBarrier;
// extern PFNGLTEXTUREBUFFERPROC gl_TextureBuffer;
// extern PFNGLTEXTUREBUFFERRANGEPROC gl_TextureBufferRange;
// extern PFNGLTEXTUREPARAMETERIIVPROC gl_TextureParameterIiv;
// extern PFNGLTEXTUREPARAMETERIUIVPROC gl_TextureParameterIuiv;
// extern PFNGLTEXTUREPARAMETERFPROC gl_TextureParameterf;
// extern PFNGLTEXTUREPARAMETERFVPROC gl_TextureParameterfv;
// extern PFNGLTEXTUREPARAMETERIPROC gl_TextureParameteri;
// extern PFNGLTEXTUREPARAMETERIVPROC gl_TextureParameteriv;
// extern PFNGLTEXTURESTORAGE1DPROC gl_TextureStorage1D;
// extern PFNGLTEXTURESTORAGE2DPROC gl_TextureStorage2D;
// extern PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC gl_TextureStorage2DMultisample;
// extern PFNGLTEXTURESTORAGE3DPROC gl_TextureStorage3D;
// extern PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC gl_TextureStorage3DMultisample;
// extern PFNGLTEXTURESUBIMAGE1DPROC gl_TextureSubImage1D;
// extern PFNGLTEXTURESUBIMAGE2DPROC gl_TextureSubImage2D;
// extern PFNGLTEXTURESUBIMAGE3DPROC gl_TextureSubImage3D;
// extern PFNGLTEXTUREVIEWPROC gl_TextureView;
// extern PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC gl_TransformFeedbackBufferBase;
// extern PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC gl_TransformFeedbackBufferRange;
// extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC gl_TransformFeedbackVaryings;
// extern PFNGLUNIFORM1DPROC gl_Uniform1d;
// extern PFNGLUNIFORM1DVPROC gl_Uniform1dv;
extern PFNGLUNIFORM1FPROC gl_Uniform1f;
// extern PFNGLUNIFORM1FVPROC gl_Uniform1fv;
extern PFNGLUNIFORM1IPROC gl_Uniform1i;
// extern PFNGLUNIFORM1IVPROC gl_Uniform1iv;
// extern PFNGLUNIFORM1UIPROC gl_Uniform1ui;
// extern PFNGLUNIFORM1UIVPROC gl_Uniform1uiv;
// extern PFNGLUNIFORM2DPROC gl_Uniform2d;
// extern PFNGLUNIFORM2DVPROC gl_Uniform2dv;
extern PFNGLUNIFORM2FPROC gl_Uniform2f;
// extern PFNGLUNIFORM2FVPROC gl_Uniform2fv;
// extern PFNGLUNIFORM2IPROC gl_Uniform2i;
// extern PFNGLUNIFORM2IVPROC gl_Uniform2iv;
// extern PFNGLUNIFORM2UIPROC gl_Uniform2ui;
// extern PFNGLUNIFORM2UIVPROC gl_Uniform2uiv;
// extern PFNGLUNIFORM3DPROC gl_Uniform3d;
// extern PFNGLUNIFORM3DVPROC gl_Uniform3dv;
// extern PFNGLUNIFORM3FPROC gl_Uniform3f;
extern PFNGLUNIFORM3FVPROC gl_Uniform3fv;
// extern PFNGLUNIFORM3IPROC gl_Uniform3i;
// extern PFNGLUNIFORM3IVPROC gl_Uniform3iv;
// extern PFNGLUNIFORM3UIPROC gl_Uniform3ui;
// extern PFNGLUNIFORM3UIVPROC gl_Uniform3uiv;
// extern PFNGLUNIFORM4DPROC gl_Uniform4d;
// extern PFNGLUNIFORM4DVPROC gl_Uniform4dv;
// extern PFNGLUNIFORM4FPROC gl_Uniform4f;
// extern PFNGLUNIFORM4FVPROC gl_Uniform4fv;
// extern PFNGLUNIFORM4IPROC gl_Uniform4i;
// extern PFNGLUNIFORM4IVPROC gl_Uniform4iv;
// extern PFNGLUNIFORM4UIPROC gl_Uniform4ui;
// extern PFNGLUNIFORM4UIVPROC gl_Uniform4uiv;
// extern PFNGLUNIFORMBLOCKBINDINGPROC gl_UniformBlockBinding;
// extern PFNGLUNIFORMHANDLEUI64ARBPROC gl_UniformHandleui64ARB;
// extern PFNGLUNIFORMHANDLEUI64VARBPROC gl_UniformHandleui64vARB;
// extern PFNGLUNIFORMMATRIX2DVPROC gl_UniformMatrix2dv;
// extern PFNGLUNIFORMMATRIX2FVPROC gl_UniformMatrix2fv;
// extern PFNGLUNIFORMMATRIX2X3DVPROC gl_UniformMatrix2x3dv;
// extern PFNGLUNIFORMMATRIX2X3FVPROC gl_UniformMatrix2x3fv;
// extern PFNGLUNIFORMMATRIX2X4DVPROC gl_UniformMatrix2x4dv;
// extern PFNGLUNIFORMMATRIX2X4FVPROC gl_UniformMatrix2x4fv;
// extern PFNGLUNIFORMMATRIX3DVPROC gl_UniformMatrix3dv;
// extern PFNGLUNIFORMMATRIX3FVPROC gl_UniformMatrix3fv;
// extern PFNGLUNIFORMMATRIX3X2DVPROC gl_UniformMatrix3x2dv;
// extern PFNGLUNIFORMMATRIX3X2FVPROC gl_UniformMatrix3x2fv;
// extern PFNGLUNIFORMMATRIX3X4DVPROC gl_UniformMatrix3x4dv;
// extern PFNGLUNIFORMMATRIX3X4FVPROC gl_UniformMatrix3x4fv;
// extern PFNGLUNIFORMMATRIX4DVPROC gl_UniformMatrix4dv;
// extern PFNGLUNIFORMMATRIX4FVPROC gl_UniformMatrix4fv;
// extern PFNGLUNIFORMMATRIX4X2DVPROC gl_UniformMatrix4x2dv;
// extern PFNGLUNIFORMMATRIX4X2FVPROC gl_UniformMatrix4x2fv;
// extern PFNGLUNIFORMMATRIX4X3DVPROC gl_UniformMatrix4x3dv;
// extern PFNGLUNIFORMMATRIX4X3FVPROC gl_UniformMatrix4x3fv;
// extern PFNGLUNIFORMSUBROUTINESUIVPROC gl_UniformSubroutinesuiv;
// extern PFNGLUNMAPBUFFERPROC gl_UnmapBuffer;
// extern PFNGLUNMAPNAMEDBUFFERPROC gl_UnmapNamedBuffer;
extern PFNGLUSEPROGRAMPROC gl_UseProgram;
// extern PFNGLUSEPROGRAMSTAGESPROC gl_UseProgramStages;
// extern PFNGLVALIDATEPROGRAMPROC gl_ValidateProgram;
// extern PFNGLVALIDATEPROGRAMPIPELINEPROC gl_ValidateProgramPipeline;
// extern PFNGLVERTEXARRAYATTRIBBINDINGPROC gl_VertexArrayAttribBinding;
// extern PFNGLVERTEXARRAYATTRIBFORMATPROC gl_VertexArrayAttribFormat;
// extern PFNGLVERTEXARRAYATTRIBIFORMATPROC gl_VertexArrayAttribIFormat;
// extern PFNGLVERTEXARRAYATTRIBLFORMATPROC gl_VertexArrayAttribLFormat;
// extern PFNGLVERTEXARRAYBINDINGDIVISORPROC gl_VertexArrayBindingDivisor;
// extern PFNGLVERTEXARRAYELEMENTBUFFERPROC gl_VertexArrayElementBuffer;
// extern PFNGLVERTEXARRAYVERTEXBUFFERPROC gl_VertexArrayVertexBuffer;
// extern PFNGLVERTEXARRAYVERTEXBUFFERSPROC gl_VertexArrayVertexBuffers;
// extern PFNGLVERTEXATTRIB1DPROC gl_VertexAttrib1d;
// extern PFNGLVERTEXATTRIB1DVPROC gl_VertexAttrib1dv;
// extern PFNGLVERTEXATTRIB1FPROC gl_VertexAttrib1f;
// extern PFNGLVERTEXATTRIB1FVPROC gl_VertexAttrib1fv;
// extern PFNGLVERTEXATTRIB1SPROC gl_VertexAttrib1s;
// extern PFNGLVERTEXATTRIB1SVPROC gl_VertexAttrib1sv;
// extern PFNGLVERTEXATTRIB2DPROC gl_VertexAttrib2d;
// extern PFNGLVERTEXATTRIB2DVPROC gl_VertexAttrib2dv;
// extern PFNGLVERTEXATTRIB2FPROC gl_VertexAttrib2f;
// extern PFNGLVERTEXATTRIB2FVPROC gl_VertexAttrib2fv;
// extern PFNGLVERTEXATTRIB2SPROC gl_VertexAttrib2s;
// extern PFNGLVERTEXATTRIB2SVPROC gl_VertexAttrib2sv;
// extern PFNGLVERTEXATTRIB3DPROC gl_VertexAttrib3d;
// extern PFNGLVERTEXATTRIB3DVPROC gl_VertexAttrib3dv;
// extern PFNGLVERTEXATTRIB3FPROC gl_VertexAttrib3f;
// extern PFNGLVERTEXATTRIB3FVPROC gl_VertexAttrib3fv;
// extern PFNGLVERTEXATTRIB3SPROC gl_VertexAttrib3s;
// extern PFNGLVERTEXATTRIB3SVPROC gl_VertexAttrib3sv;
// extern PFNGLVERTEXATTRIB4NBVPROC gl_VertexAttrib4Nbv;
// extern PFNGLVERTEXATTRIB4NIVPROC gl_VertexAttrib4Niv;
// extern PFNGLVERTEXATTRIB4NSVPROC gl_VertexAttrib4Nsv;
// extern PFNGLVERTEXATTRIB4NUBPROC gl_VertexAttrib4Nub;
// extern PFNGLVERTEXATTRIB4NUBVPROC gl_VertexAttrib4Nubv;
// extern PFNGLVERTEXATTRIB4NUIVPROC gl_VertexAttrib4Nuiv;
// extern PFNGLVERTEXATTRIB4NUSVPROC gl_VertexAttrib4Nusv;
// extern PFNGLVERTEXATTRIB4BVPROC gl_VertexAttrib4bv;
// extern PFNGLVERTEXATTRIB4DPROC gl_VertexAttrib4d;
// extern PFNGLVERTEXATTRIB4DVPROC gl_VertexAttrib4dv;
// extern PFNGLVERTEXATTRIB4FPROC gl_VertexAttrib4f;
// extern PFNGLVERTEXATTRIB4FVPROC gl_VertexAttrib4fv;
// extern PFNGLVERTEXATTRIB4IVPROC gl_VertexAttrib4iv;
// extern PFNGLVERTEXATTRIB4SPROC gl_VertexAttrib4s;
// extern PFNGLVERTEXATTRIB4SVPROC gl_VertexAttrib4sv;
// extern PFNGLVERTEXATTRIB4UBVPROC gl_VertexAttrib4ubv;
// extern PFNGLVERTEXATTRIB4UIVPROC gl_VertexAttrib4uiv;
// extern PFNGLVERTEXATTRIB4USVPROC gl_VertexAttrib4usv;
// extern PFNGLVERTEXATTRIBBINDINGPROC gl_VertexAttribBinding;
// extern PFNGLVERTEXATTRIBDIVISORPROC gl_VertexAttribDivisor;
// extern PFNGLVERTEXATTRIBFORMATPROC gl_VertexAttribFormat;
// extern PFNGLVERTEXATTRIBI1IPROC gl_VertexAttribI1i;
// extern PFNGLVERTEXATTRIBI1IVPROC gl_VertexAttribI1iv;
// extern PFNGLVERTEXATTRIBI1UIPROC gl_VertexAttribI1ui;
// extern PFNGLVERTEXATTRIBI1UIVPROC gl_VertexAttribI1uiv;
// extern PFNGLVERTEXATTRIBI2IPROC gl_VertexAttribI2i;
// extern PFNGLVERTEXATTRIBI2IVPROC gl_VertexAttribI2iv;
// extern PFNGLVERTEXATTRIBI2UIPROC gl_VertexAttribI2ui;
// extern PFNGLVERTEXATTRIBI2UIVPROC gl_VertexAttribI2uiv;
// extern PFNGLVERTEXATTRIBI3IPROC gl_VertexAttribI3i;
// extern PFNGLVERTEXATTRIBI3IVPROC gl_VertexAttribI3iv;
// extern PFNGLVERTEXATTRIBI3UIPROC gl_VertexAttribI3ui;
// extern PFNGLVERTEXATTRIBI3UIVPROC gl_VertexAttribI3uiv;
// extern PFNGLVERTEXATTRIBI4BVPROC gl_VertexAttribI4bv;
// extern PFNGLVERTEXATTRIBI4IPROC gl_VertexAttribI4i;
// extern PFNGLVERTEXATTRIBI4IVPROC gl_VertexAttribI4iv;
// extern PFNGLVERTEXATTRIBI4SVPROC gl_VertexAttribI4sv;
// extern PFNGLVERTEXATTRIBI4UBVPROC gl_VertexAttribI4ubv;
// extern PFNGLVERTEXATTRIBI4UIPROC gl_VertexAttribI4ui;
// extern PFNGLVERTEXATTRIBI4UIVPROC gl_VertexAttribI4uiv;
// extern PFNGLVERTEXATTRIBI4USVPROC gl_VertexAttribI4usv;
// extern PFNGLVERTEXATTRIBIFORMATPROC gl_VertexAttribIFormat;
// extern PFNGLVERTEXATTRIBIPOINTERPROC gl_VertexAttribIPointer;
// extern PFNGLVERTEXATTRIBL1DPROC gl_VertexAttribL1d;
// extern PFNGLVERTEXATTRIBL1DVPROC gl_VertexAttribL1dv;
// extern PFNGLVERTEXATTRIBL1UI64ARBPROC gl_VertexAttribL1ui64ARB;
// extern PFNGLVERTEXATTRIBL1UI64VARBPROC gl_VertexAttribL1ui64vARB;
// extern PFNGLVERTEXATTRIBL2DPROC gl_VertexAttribL2d;
// extern PFNGLVERTEXATTRIBL2DVPROC gl_VertexAttribL2dv;
// extern PFNGLVERTEXATTRIBL3DPROC gl_VertexAttribL3d;
// extern PFNGLVERTEXATTRIBL3DVPROC gl_VertexAttribL3dv;
// extern PFNGLVERTEXATTRIBL4DPROC gl_VertexAttribL4d;
// extern PFNGLVERTEXATTRIBL4DVPROC gl_VertexAttribL4dv;
// extern PFNGLVERTEXATTRIBLFORMATPROC gl_VertexAttribLFormat;
// extern PFNGLVERTEXATTRIBLPOINTERPROC gl_VertexAttribLPointer;
// extern PFNGLVERTEXATTRIBP1UIPROC gl_VertexAttribP1ui;
// extern PFNGLVERTEXATTRIBP1UIVPROC gl_VertexAttribP1uiv;
// extern PFNGLVERTEXATTRIBP2UIPROC gl_VertexAttribP2ui;
// extern PFNGLVERTEXATTRIBP2UIVPROC gl_VertexAttribP2uiv;
// extern PFNGLVERTEXATTRIBP3UIPROC gl_VertexAttribP3ui;
// extern PFNGLVERTEXATTRIBP3UIVPROC gl_VertexAttribP3uiv;
// extern PFNGLVERTEXATTRIBP4UIPROC gl_VertexAttribP4ui;
// extern PFNGLVERTEXATTRIBP4UIVPROC gl_VertexAttribP4uiv;
extern PFNGLVERTEXATTRIBPOINTERPROC gl_VertexAttribPointer;
// extern PFNGLVERTEXBINDINGDIVISORPROC gl_VertexBindingDivisor;
extern PFNGLVIEWPORTPROC gl_Viewport;
// extern PFNGLVIEWPORTARRAYVPROC gl_ViewportArrayv;
// extern PFNGLVIEWPORTINDEXEDFPROC gl_ViewportIndexedf;
// extern PFNGLVIEWPORTINDEXEDFVPROC gl_ViewportIndexedfv;
// extern PFNGLWAITSYNCPROC gl_WaitSync;

#define glActiveShaderProgram gl_ActiveShaderProgram
#define glActiveTexture gl_ActiveTexture
#define glAttachShader gl_AttachShader
#define glBeginConditionalRender gl_BeginConditionalRender
#define glBeginQuery gl_BeginQuery
#define glBeginQueryIndexed gl_BeginQueryIndexed
#define glBeginTransformFeedback gl_BeginTransformFeedback
#define glBindAttribLocation gl_BindAttribLocation
#define glBindBuffer gl_BindBuffer
#define glBindBufferBase gl_BindBufferBase
#define glBindBufferRange gl_BindBufferRange
#define glBindBuffersBase gl_BindBuffersBase
#define glBindBuffersRange gl_BindBuffersRange
#define glBindFragDataLocation gl_BindFragDataLocation
#define glBindFragDataLocationIndexed gl_BindFragDataLocationIndexed
#define glBindFramebuffer gl_BindFramebuffer
#define glBindImageTexture gl_BindImageTexture
#define glBindImageTextures gl_BindImageTextures
#define glBindProgramPipeline gl_BindProgramPipeline
#define glBindRenderbuffer gl_BindRenderbuffer
#define glBindSampler gl_BindSampler
#define glBindSamplers gl_BindSamplers
#define glBindTexture gl_BindTexture
#define glBindTextureUnit gl_BindTextureUnit
#define glBindTextures gl_BindTextures
#define glBindTransformFeedback gl_BindTransformFeedback
#define glBindVertexArray gl_BindVertexArray
#define glBindVertexBuffer gl_BindVertexBuffer
#define glBindVertexBuffers gl_BindVertexBuffers
#define glBlendColor gl_BlendColor
#define glBlendEquation gl_BlendEquation
#define glBlendEquationSeparate gl_BlendEquationSeparate
#define glBlendEquationSeparatei gl_BlendEquationSeparatei
#define glBlendEquationSeparateiARB gl_BlendEquationSeparateiARB
#define glBlendEquationi gl_BlendEquationi
#define glBlendEquationiARB gl_BlendEquationiARB
#define glBlendFunc gl_BlendFunc
#define glBlendFuncSeparate gl_BlendFuncSeparate
#define glBlendFuncSeparatei gl_BlendFuncSeparatei
#define glBlendFuncSeparateiARB gl_BlendFuncSeparateiARB
#define glBlendFunci gl_BlendFunci
#define glBlendFunciARB gl_BlendFunciARB
#define glBlitFramebuffer gl_BlitFramebuffer
#define glBlitNamedFramebuffer gl_BlitNamedFramebuffer
#define glBufferData gl_BufferData
#define glBufferPageCommitmentARB gl_BufferPageCommitmentARB
#define glBufferStorage gl_BufferStorage
#define glBufferSubData gl_BufferSubData
#define glCheckFramebufferStatus gl_CheckFramebufferStatus
#define glCheckNamedFramebufferStatus gl_CheckNamedFramebufferStatus
#define glClampColor gl_ClampColor
#define glClear gl_Clear
#define glClearBufferData gl_ClearBufferData
#define glClearBufferSubData gl_ClearBufferSubData
#define glClearBufferfi gl_ClearBufferfi
#define glClearBufferfv gl_ClearBufferfv
#define glClearBufferiv gl_ClearBufferiv
#define glClearBufferuiv gl_ClearBufferuiv
#define glClearColor gl_ClearColor
#define glClearDepth gl_ClearDepth
#define glClearDepthf gl_ClearDepthf
#define glClearNamedBufferData gl_ClearNamedBufferData
#define glClearNamedBufferSubData gl_ClearNamedBufferSubData
#define glClearNamedFramebufferfi gl_ClearNamedFramebufferfi
#define glClearNamedFramebufferfv gl_ClearNamedFramebufferfv
#define glClearNamedFramebufferiv gl_ClearNamedFramebufferiv
#define glClearNamedFramebufferuiv gl_ClearNamedFramebufferuiv
#define glClearStencil gl_ClearStencil
#define glClearTexImage gl_ClearTexImage
#define glClearTexSubImage gl_ClearTexSubImage
#define glClientWaitSync gl_ClientWaitSync
#define glClipControl gl_ClipControl
#define glColorMask gl_ColorMask
#define glColorMaski gl_ColorMaski
#define glCompileShader gl_CompileShader
#define glCompileShaderIncludeARB gl_CompileShaderIncludeARB
#define glCompressedTexImage1D gl_CompressedTexImage1D
#define glCompressedTexImage2D gl_CompressedTexImage2D
#define glCompressedTexImage3D gl_CompressedTexImage3D
#define glCompressedTexSubImage1D gl_CompressedTexSubImage1D
#define glCompressedTexSubImage2D gl_CompressedTexSubImage2D
#define glCompressedTexSubImage3D gl_CompressedTexSubImage3D
#define glCompressedTextureSubImage1D gl_CompressedTextureSubImage1D
#define glCompressedTextureSubImage2D gl_CompressedTextureSubImage2D
#define glCompressedTextureSubImage3D gl_CompressedTextureSubImage3D
#define glCopyBufferSubData gl_CopyBufferSubData
#define glCopyImageSubData gl_CopyImageSubData
#define glCopyNamedBufferSubData gl_CopyNamedBufferSubData
#define glCopyTexImage1D gl_CopyTexImage1D
#define glCopyTexImage2D gl_CopyTexImage2D
#define glCopyTexSubImage1D gl_CopyTexSubImage1D
#define glCopyTexSubImage2D gl_CopyTexSubImage2D
#define glCopyTexSubImage3D gl_CopyTexSubImage3D
#define glCopyTextureSubImage1D gl_CopyTextureSubImage1D
#define glCopyTextureSubImage2D gl_CopyTextureSubImage2D
#define glCopyTextureSubImage3D gl_CopyTextureSubImage3D
#define glCreateBuffers gl_CreateBuffers
#define glCreateFramebuffers gl_CreateFramebuffers
#define glCreateProgram gl_CreateProgram
#define glCreateProgramPipelines gl_CreateProgramPipelines
#define glCreateQueries gl_CreateQueries
#define glCreateRenderbuffers gl_CreateRenderbuffers
#define glCreateSamplers gl_CreateSamplers
#define glCreateShader gl_CreateShader
#define glCreateShaderProgramv gl_CreateShaderProgramv
#define glCreateSyncFromCLeventARB gl_CreateSyncFromCLeventARB
#define glCreateTextures gl_CreateTextures
#define glCreateTransformFeedbacks gl_CreateTransformFeedbacks
#define glCreateVertexArrays gl_CreateVertexArrays
#define glCullFace gl_CullFace
#define glDebugMessageCallback gl_DebugMessageCallback
#define glDebugMessageCallbackARB gl_DebugMessageCallbackARB
#define glDebugMessageControl gl_DebugMessageControl
#define glDebugMessageControlARB gl_DebugMessageControlARB
#define glDebugMessageInsert gl_DebugMessageInsert
#define glDebugMessageInsertARB gl_DebugMessageInsertARB
#define glDeleteBuffers gl_DeleteBuffers
#define glDeleteFramebuffers gl_DeleteFramebuffers
#define glDeleteNamedStringARB gl_DeleteNamedStringARB
#define glDeleteProgram gl_DeleteProgram
#define glDeleteProgramPipelines gl_DeleteProgramPipelines
#define glDeleteQueries gl_DeleteQueries
#define glDeleteRenderbuffers gl_DeleteRenderbuffers
#define glDeleteSamplers gl_DeleteSamplers
#define glDeleteShader gl_DeleteShader
#define glDeleteSync gl_DeleteSync
#define glDeleteTextures gl_DeleteTextures
#define glDeleteTransformFeedbacks gl_DeleteTransformFeedbacks
#define glDeleteVertexArrays gl_DeleteVertexArrays
#define glDepthFunc gl_DepthFunc
#define glDepthMask gl_DepthMask
#define glDepthRange gl_DepthRange
#define glDepthRangeArrayv gl_DepthRangeArrayv
#define glDepthRangeIndexed gl_DepthRangeIndexed
#define glDepthRangef gl_DepthRangef
#define glDetachShader gl_DetachShader
#define glDisable gl_Disable
#define glDisableVertexArrayAttrib gl_DisableVertexArrayAttrib
#define glDisableVertexAttribArray gl_DisableVertexAttribArray
#define glDisablei gl_Disablei
#define glDispatchCompute gl_DispatchCompute
#define glDispatchComputeGroupSizeARB gl_DispatchComputeGroupSizeARB
#define glDispatchComputeIndirect gl_DispatchComputeIndirect
#define glDrawArrays gl_DrawArrays
#define glDrawArraysIndirect gl_DrawArraysIndirect
#define glDrawArraysInstanced gl_DrawArraysInstanced
#define glDrawArraysInstancedBaseInstance gl_DrawArraysInstancedBaseInstance
#define glDrawBuffer gl_DrawBuffer
#define glDrawBuffers gl_DrawBuffers
#define glDrawElements gl_DrawElements
#define glDrawElementsBaseVertex gl_DrawElementsBaseVertex
#define glDrawElementsIndirect gl_DrawElementsIndirect
#define glDrawElementsInstanced gl_DrawElementsInstanced
#define glDrawElementsInstancedBaseInstance gl_DrawElementsInstancedBaseInstance
#define glDrawElementsInstancedBaseVertex gl_DrawElementsInstancedBaseVertex
#define glDrawElementsInstancedBaseVertexBaseInstance gl_DrawElementsInstancedBaseVertexBaseInstance
#define glDrawRangeElements gl_DrawRangeElements
#define glDrawRangeElementsBaseVertex gl_DrawRangeElementsBaseVertex
#define glDrawTransformFeedback gl_DrawTransformFeedback
#define glDrawTransformFeedbackInstanced gl_DrawTransformFeedbackInstanced
#define glDrawTransformFeedbackStream gl_DrawTransformFeedbackStream
#define glDrawTransformFeedbackStreamInstanced gl_DrawTransformFeedbackStreamInstanced
#define glEnable gl_Enable
#define glEnableVertexArrayAttrib gl_EnableVertexArrayAttrib
#define glEnableVertexAttribArray gl_EnableVertexAttribArray
#define glEnablei gl_Enablei
#define glEndConditionalRender gl_EndConditionalRender
#define glEndQuery gl_EndQuery
#define glEndQueryIndexed gl_EndQueryIndexed
#define glEndTransformFeedback gl_EndTransformFeedback
#define glFenceSync gl_FenceSync
#define glFinish gl_Finish
#define glFlush gl_Flush
#define glFlushMappedBufferRange gl_FlushMappedBufferRange
#define glFlushMappedNamedBufferRange gl_FlushMappedNamedBufferRange
#define glFramebufferParameteri gl_FramebufferParameteri
#define glFramebufferRenderbuffer gl_FramebufferRenderbuffer
#define glFramebufferTexture gl_FramebufferTexture
#define glFramebufferTexture1D gl_FramebufferTexture1D
#define glFramebufferTexture2D gl_FramebufferTexture2D
#define glFramebufferTexture3D gl_FramebufferTexture3D
#define glFramebufferTextureLayer gl_FramebufferTextureLayer
#define glFrontFace gl_FrontFace
#define glGenBuffers gl_GenBuffers
#define glGenFramebuffers gl_GenFramebuffers
#define glGenProgramPipelines gl_GenProgramPipelines
#define glGenQueries gl_GenQueries
#define glGenRenderbuffers gl_GenRenderbuffers
#define glGenSamplers gl_GenSamplers
#define glGenTextures gl_GenTextures
#define glGenTransformFeedbacks gl_GenTransformFeedbacks
#define glGenVertexArrays gl_GenVertexArrays
#define glGenerateMipmap gl_GenerateMipmap
#define glGenerateTextureMipmap gl_GenerateTextureMipmap
#define glGetActiveAtomicCounterBufferiv gl_GetActiveAtomicCounterBufferiv
#define glGetActiveAttrib gl_GetActiveAttrib
#define glGetActiveSubroutineName gl_GetActiveSubroutineName
#define glGetActiveSubroutineUniformName gl_GetActiveSubroutineUniformName
#define glGetActiveSubroutineUniformiv gl_GetActiveSubroutineUniformiv
#define glGetActiveUniform gl_GetActiveUniform
#define glGetActiveUniformBlockName gl_GetActiveUniformBlockName
#define glGetActiveUniformBlockiv gl_GetActiveUniformBlockiv
#define glGetActiveUniformName gl_GetActiveUniformName
#define glGetActiveUniformsiv gl_GetActiveUniformsiv
#define glGetAttachedShaders gl_GetAttachedShaders
#define glGetAttribLocation gl_GetAttribLocation
#define glGetBooleani_v gl_GetBooleani_v
#define glGetBooleanv gl_GetBooleanv
#define glGetBufferParameteri64v gl_GetBufferParameteri64v
#define glGetBufferParameteriv gl_GetBufferParameteriv
#define glGetBufferPointerv gl_GetBufferPointerv
#define glGetBufferSubData gl_GetBufferSubData
#define glGetCompressedTexImage gl_GetCompressedTexImage
#define glGetCompressedTextureImage gl_GetCompressedTextureImage
#define glGetCompressedTextureSubImage gl_GetCompressedTextureSubImage
#define glGetDebugMessageLog gl_GetDebugMessageLog
#define glGetDebugMessageLogARB gl_GetDebugMessageLogARB
#define glGetDoublei_v gl_GetDoublei_v
#define glGetDoublev gl_GetDoublev
#define glGetError gl_GetError
#define glGetFloati_v gl_GetFloati_v
#define glGetFloatv gl_GetFloatv
#define glGetFragDataIndex gl_GetFragDataIndex
#define glGetFragDataLocation gl_GetFragDataLocation
#define glGetFramebufferAttachmentParameteriv gl_GetFramebufferAttachmentParameteriv
#define glGetFramebufferParameteriv gl_GetFramebufferParameteriv
#define glGetGraphicsResetStatus gl_GetGraphicsResetStatus
#define glGetGraphicsResetStatusARB gl_GetGraphicsResetStatusARB
#define glGetImageHandleARB gl_GetImageHandleARB
#define glGetInteger64i_v gl_GetInteger64i_v
#define glGetInteger64v gl_GetInteger64v
#define glGetIntegeri_v gl_GetIntegeri_v
#define glGetIntegerv gl_GetIntegerv
#define glGetInternalformati64v gl_GetInternalformati64v
#define glGetInternalformativ gl_GetInternalformativ
#define glGetMultisamplefv gl_GetMultisamplefv
#define glGetNamedBufferParameteri64v gl_GetNamedBufferParameteri64v
#define glGetNamedBufferParameteriv gl_GetNamedBufferParameteriv
#define glGetNamedBufferPointerv gl_GetNamedBufferPointerv
#define glGetNamedBufferSubData gl_GetNamedBufferSubData
#define glGetNamedFramebufferAttachmentParameteriv gl_GetNamedFramebufferAttachmentParameteriv
#define glGetNamedFramebufferParameteriv gl_GetNamedFramebufferParameteriv
#define glGetNamedRenderbufferParameteriv gl_GetNamedRenderbufferParameteriv
#define glGetNamedStringARB gl_GetNamedStringARB
#define glGetNamedStringivARB gl_GetNamedStringivARB
#define glGetObjectLabel gl_GetObjectLabel
#define glGetObjectPtrLabel gl_GetObjectPtrLabel
#define glGetPointerv gl_GetPointerv
#define glGetProgramBinary gl_GetProgramBinary
#define glGetProgramInfoLog gl_GetProgramInfoLog
#define glGetProgramInterfaceiv gl_GetProgramInterfaceiv
#define glGetProgramPipelineInfoLog gl_GetProgramPipelineInfoLog
#define glGetProgramPipelineiv gl_GetProgramPipelineiv
#define glGetProgramResourceIndex gl_GetProgramResourceIndex
#define glGetProgramResourceLocation gl_GetProgramResourceLocation
#define glGetProgramResourceLocationIndex gl_GetProgramResourceLocationIndex
#define glGetProgramResourceName gl_GetProgramResourceName
#define glGetProgramResourceiv gl_GetProgramResourceiv
#define glGetProgramStageiv gl_GetProgramStageiv
#define glGetProgramiv gl_GetProgramiv
#define glGetQueryBufferObjecti64v gl_GetQueryBufferObjecti64v
#define glGetQueryBufferObjectiv gl_GetQueryBufferObjectiv
#define glGetQueryBufferObjectui64v gl_GetQueryBufferObjectui64v
#define glGetQueryBufferObjectuiv gl_GetQueryBufferObjectuiv
#define glGetQueryIndexediv gl_GetQueryIndexediv
#define glGetQueryObjecti64v gl_GetQueryObjecti64v
#define glGetQueryObjectiv gl_GetQueryObjectiv
#define glGetQueryObjectui64v gl_GetQueryObjectui64v
#define glGetQueryObjectuiv gl_GetQueryObjectuiv
#define glGetQueryiv gl_GetQueryiv
#define glGetRenderbufferParameteriv gl_GetRenderbufferParameteriv
#define glGetSamplerParameterIiv gl_GetSamplerParameterIiv
#define glGetSamplerParameterIuiv gl_GetSamplerParameterIuiv
#define glGetSamplerParameterfv gl_GetSamplerParameterfv
#define glGetSamplerParameteriv gl_GetSamplerParameteriv
#define glGetShaderInfoLog gl_GetShaderInfoLog
#define glGetShaderPrecisionFormat gl_GetShaderPrecisionFormat
#define glGetShaderSource gl_GetShaderSource
#define glGetShaderiv gl_GetShaderiv
#define glGetString gl_GetString
#define glGetStringi gl_GetStringi
#define glGetSubroutineIndex gl_GetSubroutineIndex
#define glGetSubroutineUniformLocation gl_GetSubroutineUniformLocation
#define glGetSynciv gl_GetSynciv
#define glGetTexImage gl_GetTexImage
#define glGetTexLevelParameterfv gl_GetTexLevelParameterfv
#define glGetTexLevelParameteriv gl_GetTexLevelParameteriv
#define glGetTexParameterIiv gl_GetTexParameterIiv
#define glGetTexParameterIuiv gl_GetTexParameterIuiv
#define glGetTexParameterfv gl_GetTexParameterfv
#define glGetTexParameteriv gl_GetTexParameteriv
#define glGetTextureHandleARB gl_GetTextureHandleARB
#define glGetTextureImage gl_GetTextureImage
#define glGetTextureLevelParameterfv gl_GetTextureLevelParameterfv
#define glGetTextureLevelParameteriv gl_GetTextureLevelParameteriv
#define glGetTextureParameterIiv gl_GetTextureParameterIiv
#define glGetTextureParameterIuiv gl_GetTextureParameterIuiv
#define glGetTextureParameterfv gl_GetTextureParameterfv
#define glGetTextureParameteriv gl_GetTextureParameteriv
#define glGetTextureSamplerHandleARB gl_GetTextureSamplerHandleARB
#define glGetTextureSubImage gl_GetTextureSubImage
#define glGetTransformFeedbackVarying gl_GetTransformFeedbackVarying
#define glGetTransformFeedbacki64_v gl_GetTransformFeedbacki64_v
#define glGetTransformFeedbacki_v gl_GetTransformFeedbacki_v
#define glGetTransformFeedbackiv gl_GetTransformFeedbackiv
#define glGetUniformBlockIndex gl_GetUniformBlockIndex
#define glGetUniformIndices gl_GetUniformIndices
#define glGetUniformLocation gl_GetUniformLocation
#define glGetUniformSubroutineuiv gl_GetUniformSubroutineuiv
#define glGetUniformdv gl_GetUniformdv
#define glGetUniformfv gl_GetUniformfv
#define glGetUniformiv gl_GetUniformiv
#define glGetUniformuiv gl_GetUniformuiv
#define glGetVertexArrayIndexed64iv gl_GetVertexArrayIndexed64iv
#define glGetVertexArrayIndexediv gl_GetVertexArrayIndexediv
#define glGetVertexArrayiv gl_GetVertexArrayiv
#define glGetVertexAttribIiv gl_GetVertexAttribIiv
#define glGetVertexAttribIuiv gl_GetVertexAttribIuiv
#define glGetVertexAttribLdv gl_GetVertexAttribLdv
#define glGetVertexAttribLui64vARB gl_GetVertexAttribLui64vARB
#define glGetVertexAttribPointerv gl_GetVertexAttribPointerv
#define glGetVertexAttribdv gl_GetVertexAttribdv
#define glGetVertexAttribfv gl_GetVertexAttribfv
#define glGetVertexAttribiv gl_GetVertexAttribiv
#define glGetnCompressedTexImage gl_GetnCompressedTexImage
#define glGetnCompressedTexImageARB gl_GetnCompressedTexImageARB
#define glGetnTexImage gl_GetnTexImage
#define glGetnTexImageARB gl_GetnTexImageARB
#define glGetnUniformdv gl_GetnUniformdv
#define glGetnUniformdvARB gl_GetnUniformdvARB
#define glGetnUniformfv gl_GetnUniformfv
#define glGetnUniformfvARB gl_GetnUniformfvARB
#define glGetnUniformiv gl_GetnUniformiv
#define glGetnUniformivARB gl_GetnUniformivARB
#define glGetnUniformuiv gl_GetnUniformuiv
#define glGetnUniformuivARB gl_GetnUniformuivARB
#define glHint gl_Hint
#define glInvalidateBufferData gl_InvalidateBufferData
#define glInvalidateBufferSubData gl_InvalidateBufferSubData
#define glInvalidateFramebuffer gl_InvalidateFramebuffer
#define glInvalidateNamedFramebufferData gl_InvalidateNamedFramebufferData
#define glInvalidateNamedFramebufferSubData gl_InvalidateNamedFramebufferSubData
#define glInvalidateSubFramebuffer gl_InvalidateSubFramebuffer
#define glInvalidateTexImage gl_InvalidateTexImage
#define glInvalidateTexSubImage gl_InvalidateTexSubImage
#define glIsBuffer gl_IsBuffer
#define glIsEnabled gl_IsEnabled
#define glIsEnabledi gl_IsEnabledi
#define glIsFramebuffer gl_IsFramebuffer
#define glIsImageHandleResidentARB gl_IsImageHandleResidentARB
#define glIsNamedStringARB gl_IsNamedStringARB
#define glIsProgram gl_IsProgram
#define glIsProgramPipeline gl_IsProgramPipeline
#define glIsQuery gl_IsQuery
#define glIsRenderbuffer gl_IsRenderbuffer
#define glIsSampler gl_IsSampler
#define glIsShader gl_IsShader
#define glIsSync gl_IsSync
#define glIsTexture gl_IsTexture
#define glIsTextureHandleResidentARB gl_IsTextureHandleResidentARB
#define glIsTransformFeedback gl_IsTransformFeedback
#define glIsVertexArray gl_IsVertexArray
#define glLineWidth gl_LineWidth
#define glLinkProgram gl_LinkProgram
#define glLogicOp gl_LogicOp
#define glMakeImageHandleNonResidentARB gl_MakeImageHandleNonResidentARB
#define glMakeImageHandleResidentARB gl_MakeImageHandleResidentARB
#define glMakeTextureHandleNonResidentARB gl_MakeTextureHandleNonResidentARB
#define glMakeTextureHandleResidentARB gl_MakeTextureHandleResidentARB
#define glMapBuffer gl_MapBuffer
#define glMapBufferRange gl_MapBufferRange
#define glMapNamedBuffer gl_MapNamedBuffer
#define glMapNamedBufferRange gl_MapNamedBufferRange
#define glMemoryBarrier gl_MemoryBarrier
#define glMemoryBarrierByRegion gl_MemoryBarrierByRegion
#define glMinSampleShading gl_MinSampleShading
#define glMinSampleShadingARB gl_MinSampleShadingARB
#define glMultiDrawArrays gl_MultiDrawArrays
#define glMultiDrawArraysIndirect gl_MultiDrawArraysIndirect
#define glMultiDrawArraysIndirectCountARB gl_MultiDrawArraysIndirectCountARB
#define glMultiDrawElements gl_MultiDrawElements
#define glMultiDrawElementsBaseVertex gl_MultiDrawElementsBaseVertex
#define glMultiDrawElementsIndirect gl_MultiDrawElementsIndirect
#define glMultiDrawElementsIndirectCountARB gl_MultiDrawElementsIndirectCountARB
#define glNamedBufferData gl_NamedBufferData
#define glNamedBufferPageCommitmentARB gl_NamedBufferPageCommitmentARB
#define glNamedBufferPageCommitmentEXT gl_NamedBufferPageCommitmentEXT
#define glNamedBufferStorage gl_NamedBufferStorage
#define glNamedBufferSubData gl_NamedBufferSubData
#define glNamedFramebufferDrawBuffer gl_NamedFramebufferDrawBuffer
#define glNamedFramebufferDrawBuffers gl_NamedFramebufferDrawBuffers
#define glNamedFramebufferParameteri gl_NamedFramebufferParameteri
#define glNamedFramebufferReadBuffer gl_NamedFramebufferReadBuffer
#define glNamedFramebufferRenderbuffer gl_NamedFramebufferRenderbuffer
#define glNamedFramebufferTexture gl_NamedFramebufferTexture
#define glNamedFramebufferTextureLayer gl_NamedFramebufferTextureLayer
#define glNamedRenderbufferStorage gl_NamedRenderbufferStorage
#define glNamedRenderbufferStorageMultisample gl_NamedRenderbufferStorageMultisample
#define glNamedStringARB gl_NamedStringARB
#define glObjectLabel gl_ObjectLabel
#define glObjectPtrLabel gl_ObjectPtrLabel
#define glPatchParameterfv gl_PatchParameterfv
#define glPatchParameteri gl_PatchParameteri
#define glPauseTransformFeedback gl_PauseTransformFeedback
#define glPixelStoref gl_PixelStoref
#define glPixelStorei gl_PixelStorei
#define glPointParameterf gl_PointParameterf
#define glPointParameterfv gl_PointParameterfv
#define glPointParameteri gl_PointParameteri
#define glPointParameteriv gl_PointParameteriv
#define glPointSize gl_PointSize
#define glPolygonMode gl_PolygonMode
#define glPolygonOffset gl_PolygonOffset
#define glPopDebugGroup gl_PopDebugGroup
#define glPrimitiveRestartIndex gl_PrimitiveRestartIndex
#define glProgramBinary gl_ProgramBinary
#define glProgramParameteri gl_ProgramParameteri
#define glProgramUniform1d gl_ProgramUniform1d
#define glProgramUniform1dv gl_ProgramUniform1dv
#define glProgramUniform1f gl_ProgramUniform1f
#define glProgramUniform1fv gl_ProgramUniform1fv
#define glProgramUniform1i gl_ProgramUniform1i
#define glProgramUniform1iv gl_ProgramUniform1iv
#define glProgramUniform1ui gl_ProgramUniform1ui
#define glProgramUniform1uiv gl_ProgramUniform1uiv
#define glProgramUniform2d gl_ProgramUniform2d
#define glProgramUniform2dv gl_ProgramUniform2dv
#define glProgramUniform2f gl_ProgramUniform2f
#define glProgramUniform2fv gl_ProgramUniform2fv
#define glProgramUniform2i gl_ProgramUniform2i
#define glProgramUniform2iv gl_ProgramUniform2iv
#define glProgramUniform2ui gl_ProgramUniform2ui
#define glProgramUniform2uiv gl_ProgramUniform2uiv
#define glProgramUniform3d gl_ProgramUniform3d
#define glProgramUniform3dv gl_ProgramUniform3dv
#define glProgramUniform3f gl_ProgramUniform3f
#define glProgramUniform3fv gl_ProgramUniform3fv
#define glProgramUniform3i gl_ProgramUniform3i
#define glProgramUniform3iv gl_ProgramUniform3iv
#define glProgramUniform3ui gl_ProgramUniform3ui
#define glProgramUniform3uiv gl_ProgramUniform3uiv
#define glProgramUniform4d gl_ProgramUniform4d
#define glProgramUniform4dv gl_ProgramUniform4dv
#define glProgramUniform4f gl_ProgramUniform4f
#define glProgramUniform4fv gl_ProgramUniform4fv
#define glProgramUniform4i gl_ProgramUniform4i
#define glProgramUniform4iv gl_ProgramUniform4iv
#define glProgramUniform4ui gl_ProgramUniform4ui
#define glProgramUniform4uiv gl_ProgramUniform4uiv
#define glProgramUniformHandleui64ARB gl_ProgramUniformHandleui64ARB
#define glProgramUniformHandleui64vARB gl_ProgramUniformHandleui64vARB
#define glProgramUniformMatrix2dv gl_ProgramUniformMatrix2dv
#define glProgramUniformMatrix2fv gl_ProgramUniformMatrix2fv
#define glProgramUniformMatrix2x3dv gl_ProgramUniformMatrix2x3dv
#define glProgramUniformMatrix2x3fv gl_ProgramUniformMatrix2x3fv
#define glProgramUniformMatrix2x4dv gl_ProgramUniformMatrix2x4dv
#define glProgramUniformMatrix2x4fv gl_ProgramUniformMatrix2x4fv
#define glProgramUniformMatrix3dv gl_ProgramUniformMatrix3dv
#define glProgramUniformMatrix3fv gl_ProgramUniformMatrix3fv
#define glProgramUniformMatrix3x2dv gl_ProgramUniformMatrix3x2dv
#define glProgramUniformMatrix3x2fv gl_ProgramUniformMatrix3x2fv
#define glProgramUniformMatrix3x4dv gl_ProgramUniformMatrix3x4dv
#define glProgramUniformMatrix3x4fv gl_ProgramUniformMatrix3x4fv
#define glProgramUniformMatrix4dv gl_ProgramUniformMatrix4dv
#define glProgramUniformMatrix4fv gl_ProgramUniformMatrix4fv
#define glProgramUniformMatrix4x2dv gl_ProgramUniformMatrix4x2dv
#define glProgramUniformMatrix4x2fv gl_ProgramUniformMatrix4x2fv
#define glProgramUniformMatrix4x3dv gl_ProgramUniformMatrix4x3dv
#define glProgramUniformMatrix4x3fv gl_ProgramUniformMatrix4x3fv
#define glProvokingVertex gl_ProvokingVertex
#define glPushDebugGroup gl_PushDebugGroup
#define glQueryCounter gl_QueryCounter
#define glReadBuffer gl_ReadBuffer
#define glReadPixels gl_ReadPixels
#define glReadnPixels gl_ReadnPixels
#define glReadnPixelsARB gl_ReadnPixelsARB
#define glReleaseShaderCompiler gl_ReleaseShaderCompiler
#define glRenderbufferStorage gl_RenderbufferStorage
#define glRenderbufferStorageMultisample gl_RenderbufferStorageMultisample
#define glResumeTransformFeedback gl_ResumeTransformFeedback
#define glSampleCoverage gl_SampleCoverage
#define glSampleMaski gl_SampleMaski
#define glSamplerParameterIiv gl_SamplerParameterIiv
#define glSamplerParameterIuiv gl_SamplerParameterIuiv
#define glSamplerParameterf gl_SamplerParameterf
#define glSamplerParameterfv gl_SamplerParameterfv
#define glSamplerParameteri gl_SamplerParameteri
#define glSamplerParameteriv gl_SamplerParameteriv
#define glScissor gl_Scissor
#define glScissorArrayv gl_ScissorArrayv
#define glScissorIndexed gl_ScissorIndexed
#define glScissorIndexedv gl_ScissorIndexedv
#define glShaderBinary gl_ShaderBinary
#define glShaderSource gl_ShaderSource
#define glShaderStorageBlockBinding gl_ShaderStorageBlockBinding
#define glStencilFunc gl_StencilFunc
#define glStencilFuncSeparate gl_StencilFuncSeparate
#define glStencilMask gl_StencilMask
#define glStencilMaskSeparate gl_StencilMaskSeparate
#define glStencilOp gl_StencilOp
#define glStencilOpSeparate gl_StencilOpSeparate
#define glTexBuffer gl_TexBuffer
#define glTexBufferRange gl_TexBufferRange
#define glTexImage1D gl_TexImage1D
#define glTexImage2D gl_TexImage2D
#define glTexImage2DMultisample gl_TexImage2DMultisample
#define glTexImage3D gl_TexImage3D
#define glTexImage3DMultisample gl_TexImage3DMultisample
#define glTexPageCommitmentARB gl_TexPageCommitmentARB
#define glTexParameterIiv gl_TexParameterIiv
#define glTexParameterIuiv gl_TexParameterIuiv
#define glTexParameterf gl_TexParameterf
#define glTexParameterfv gl_TexParameterfv
#define glTexParameteri gl_TexParameteri
#define glTexParameteriv gl_TexParameteriv
#define glTexStorage1D gl_TexStorage1D
#define glTexStorage2D gl_TexStorage2D
#define glTexStorage2DMultisample gl_TexStorage2DMultisample
#define glTexStorage3D gl_TexStorage3D
#define glTexStorage3DMultisample gl_TexStorage3DMultisample
#define glTexSubImage1D gl_TexSubImage1D
#define glTexSubImage2D gl_TexSubImage2D
#define glTexSubImage3D gl_TexSubImage3D
#define glTextureBarrier gl_TextureBarrier
#define glTextureBuffer gl_TextureBuffer
#define glTextureBufferRange gl_TextureBufferRange
#define glTextureParameterIiv gl_TextureParameterIiv
#define glTextureParameterIuiv gl_TextureParameterIuiv
#define glTextureParameterf gl_TextureParameterf
#define glTextureParameterfv gl_TextureParameterfv
#define glTextureParameteri gl_TextureParameteri
#define glTextureParameteriv gl_TextureParameteriv
#define glTextureStorage1D gl_TextureStorage1D
#define glTextureStorage2D gl_TextureStorage2D
#define glTextureStorage2DMultisample gl_TextureStorage2DMultisample
#define glTextureStorage3D gl_TextureStorage3D
#define glTextureStorage3DMultisample gl_TextureStorage3DMultisample
#define glTextureSubImage1D gl_TextureSubImage1D
#define glTextureSubImage2D gl_TextureSubImage2D
#define glTextureSubImage3D gl_TextureSubImage3D
#define glTextureView gl_TextureView
#define glTransformFeedbackBufferBase gl_TransformFeedbackBufferBase
#define glTransformFeedbackBufferRange gl_TransformFeedbackBufferRange
#define glTransformFeedbackVaryings gl_TransformFeedbackVaryings
#define glUniform1d gl_Uniform1d
#define glUniform1dv gl_Uniform1dv
#define glUniform1f gl_Uniform1f
#define glUniform1fv gl_Uniform1fv
#define glUniform1i gl_Uniform1i
#define glUniform1iv gl_Uniform1iv
#define glUniform1ui gl_Uniform1ui
#define glUniform1uiv gl_Uniform1uiv
#define glUniform2d gl_Uniform2d
#define glUniform2dv gl_Uniform2dv
#define glUniform2f gl_Uniform2f
#define glUniform2fv gl_Uniform2fv
#define glUniform2i gl_Uniform2i
#define glUniform2iv gl_Uniform2iv
#define glUniform2ui gl_Uniform2ui
#define glUniform2uiv gl_Uniform2uiv
#define glUniform3d gl_Uniform3d
#define glUniform3dv gl_Uniform3dv
#define glUniform3f gl_Uniform3f
#define glUniform3fv gl_Uniform3fv
#define glUniform3i gl_Uniform3i
#define glUniform3iv gl_Uniform3iv
#define glUniform3ui gl_Uniform3ui
#define glUniform3uiv gl_Uniform3uiv
#define glUniform4d gl_Uniform4d
#define glUniform4dv gl_Uniform4dv
#define glUniform4f gl_Uniform4f
#define glUniform4fv gl_Uniform4fv
#define glUniform4i gl_Uniform4i
#define glUniform4iv gl_Uniform4iv
#define glUniform4ui gl_Uniform4ui
#define glUniform4uiv gl_Uniform4uiv
#define glUniformBlockBinding gl_UniformBlockBinding
#define glUniformHandleui64ARB gl_UniformHandleui64ARB
#define glUniformHandleui64vARB gl_UniformHandleui64vARB
#define glUniformMatrix2dv gl_UniformMatrix2dv
#define glUniformMatrix2fv gl_UniformMatrix2fv
#define glUniformMatrix2x3dv gl_UniformMatrix2x3dv
#define glUniformMatrix2x3fv gl_UniformMatrix2x3fv
#define glUniformMatrix2x4dv gl_UniformMatrix2x4dv
#define glUniformMatrix2x4fv gl_UniformMatrix2x4fv
#define glUniformMatrix3dv gl_UniformMatrix3dv
#define glUniformMatrix3fv gl_UniformMatrix3fv
#define glUniformMatrix3x2dv gl_UniformMatrix3x2dv
#define glUniformMatrix3x2fv gl_UniformMatrix3x2fv
#define glUniformMatrix3x4dv gl_UniformMatrix3x4dv
#define glUniformMatrix3x4fv gl_UniformMatrix3x4fv
#define glUniformMatrix4dv gl_UniformMatrix4dv
#define glUniformMatrix4fv gl_UniformMatrix4fv
#define glUniformMatrix4x2dv gl_UniformMatrix4x2dv
#define glUniformMatrix4x2fv gl_UniformMatrix4x2fv
#define glUniformMatrix4x3dv gl_UniformMatrix4x3dv
#define glUniformMatrix4x3fv gl_UniformMatrix4x3fv
#define glUniformSubroutinesuiv gl_UniformSubroutinesuiv
#define glUnmapBuffer gl_UnmapBuffer
#define glUnmapNamedBuffer gl_UnmapNamedBuffer
#define glUseProgram gl_UseProgram
#define glUseProgramStages gl_UseProgramStages
#define glValidateProgram gl_ValidateProgram
#define glValidateProgramPipeline gl_ValidateProgramPipeline
#define glVertexArrayAttribBinding gl_VertexArrayAttribBinding
#define glVertexArrayAttribFormat gl_VertexArrayAttribFormat
#define glVertexArrayAttribIFormat gl_VertexArrayAttribIFormat
#define glVertexArrayAttribLFormat gl_VertexArrayAttribLFormat
#define glVertexArrayBindingDivisor gl_VertexArrayBindingDivisor
#define glVertexArrayElementBuffer gl_VertexArrayElementBuffer
#define glVertexArrayVertexBuffer gl_VertexArrayVertexBuffer
#define glVertexArrayVertexBuffers gl_VertexArrayVertexBuffers
#define glVertexAttrib1d gl_VertexAttrib1d
#define glVertexAttrib1dv gl_VertexAttrib1dv
#define glVertexAttrib1f gl_VertexAttrib1f
#define glVertexAttrib1fv gl_VertexAttrib1fv
#define glVertexAttrib1s gl_VertexAttrib1s
#define glVertexAttrib1sv gl_VertexAttrib1sv
#define glVertexAttrib2d gl_VertexAttrib2d
#define glVertexAttrib2dv gl_VertexAttrib2dv
#define glVertexAttrib2f gl_VertexAttrib2f
#define glVertexAttrib2fv gl_VertexAttrib2fv
#define glVertexAttrib2s gl_VertexAttrib2s
#define glVertexAttrib2sv gl_VertexAttrib2sv
#define glVertexAttrib3d gl_VertexAttrib3d
#define glVertexAttrib3dv gl_VertexAttrib3dv
#define glVertexAttrib3f gl_VertexAttrib3f
#define glVertexAttrib3fv gl_VertexAttrib3fv
#define glVertexAttrib3s gl_VertexAttrib3s
#define glVertexAttrib3sv gl_VertexAttrib3sv
#define glVertexAttrib4Nbv gl_VertexAttrib4Nbv
#define glVertexAttrib4Niv gl_VertexAttrib4Niv
#define glVertexAttrib4Nsv gl_VertexAttrib4Nsv
#define glVertexAttrib4Nub gl_VertexAttrib4Nub
#define glVertexAttrib4Nubv gl_VertexAttrib4Nubv
#define glVertexAttrib4Nuiv gl_VertexAttrib4Nuiv
#define glVertexAttrib4Nusv gl_VertexAttrib4Nusv
#define glVertexAttrib4bv gl_VertexAttrib4bv
#define glVertexAttrib4d gl_VertexAttrib4d
#define glVertexAttrib4dv gl_VertexAttrib4dv
#define glVertexAttrib4f gl_VertexAttrib4f
#define glVertexAttrib4fv gl_VertexAttrib4fv
#define glVertexAttrib4iv gl_VertexAttrib4iv
#define glVertexAttrib4s gl_VertexAttrib4s
#define glVertexAttrib4sv gl_VertexAttrib4sv
#define glVertexAttrib4ubv gl_VertexAttrib4ubv
#define glVertexAttrib4uiv gl_VertexAttrib4uiv
#define glVertexAttrib4usv gl_VertexAttrib4usv
#define glVertexAttribBinding gl_VertexAttribBinding
#define glVertexAttribDivisor gl_VertexAttribDivisor
#define glVertexAttribFormat gl_VertexAttribFormat
#define glVertexAttribI1i gl_VertexAttribI1i
#define glVertexAttribI1iv gl_VertexAttribI1iv
#define glVertexAttribI1ui gl_VertexAttribI1ui
#define glVertexAttribI1uiv gl_VertexAttribI1uiv
#define glVertexAttribI2i gl_VertexAttribI2i
#define glVertexAttribI2iv gl_VertexAttribI2iv
#define glVertexAttribI2ui gl_VertexAttribI2ui
#define glVertexAttribI2uiv gl_VertexAttribI2uiv
#define glVertexAttribI3i gl_VertexAttribI3i
#define glVertexAttribI3iv gl_VertexAttribI3iv
#define glVertexAttribI3ui gl_VertexAttribI3ui
#define glVertexAttribI3uiv gl_VertexAttribI3uiv
#define glVertexAttribI4bv gl_VertexAttribI4bv
#define glVertexAttribI4i gl_VertexAttribI4i
#define glVertexAttribI4iv gl_VertexAttribI4iv
#define glVertexAttribI4sv gl_VertexAttribI4sv
#define glVertexAttribI4ubv gl_VertexAttribI4ubv
#define glVertexAttribI4ui gl_VertexAttribI4ui
#define glVertexAttribI4uiv gl_VertexAttribI4uiv
#define glVertexAttribI4usv gl_VertexAttribI4usv
#define glVertexAttribIFormat gl_VertexAttribIFormat
#define glVertexAttribIPointer gl_VertexAttribIPointer
#define glVertexAttribL1d gl_VertexAttribL1d
#define glVertexAttribL1dv gl_VertexAttribL1dv
#define glVertexAttribL1ui64ARB gl_VertexAttribL1ui64ARB
#define glVertexAttribL1ui64vARB gl_VertexAttribL1ui64vARB
#define glVertexAttribL2d gl_VertexAttribL2d
#define glVertexAttribL2dv gl_VertexAttribL2dv
#define glVertexAttribL3d gl_VertexAttribL3d
#define glVertexAttribL3dv gl_VertexAttribL3dv
#define glVertexAttribL4d gl_VertexAttribL4d
#define glVertexAttribL4dv gl_VertexAttribL4dv
#define glVertexAttribLFormat gl_VertexAttribLFormat
#define glVertexAttribLPointer gl_VertexAttribLPointer
#define glVertexAttribP1ui gl_VertexAttribP1ui
#define glVertexAttribP1uiv gl_VertexAttribP1uiv
#define glVertexAttribP2ui gl_VertexAttribP2ui
#define glVertexAttribP2uiv gl_VertexAttribP2uiv
#define glVertexAttribP3ui gl_VertexAttribP3ui
#define glVertexAttribP3uiv gl_VertexAttribP3uiv
#define glVertexAttribP4ui gl_VertexAttribP4ui
#define glVertexAttribP4uiv gl_VertexAttribP4uiv
#define glVertexAttribPointer gl_VertexAttribPointer
#define glVertexBindingDivisor gl_VertexBindingDivisor
#define glViewport gl_Viewport
#define glViewportArrayv gl_ViewportArrayv
#define glViewportIndexedf gl_ViewportIndexedf
#define glViewportIndexedfv gl_ViewportIndexedfv
#define glWaitSync gl_WaitSync
