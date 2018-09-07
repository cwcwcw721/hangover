/*
 * Copyright 2017 André Hentschel
 * Copyright 2018 Stefan Dösinger for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#define COBJMACROS

#include <initguid.h>
#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#include "windows-user-services.h"
#include "dll_list.h"

#include <wine/debug.h>
#include <wine/list.h>

#include "qemu_windowscodecs.h"

WINE_DEFAULT_DEBUG_CHANNEL(qemu_wic);

struct qemu_dll_init
{
    struct qemu_syscall super;
    uint64_t reason;
};

#ifdef QEMU_DLL_GUEST

BOOL WINAPI DllMain(HMODULE mod, DWORD reason, void *reserved)
{
    struct qemu_dll_init call;
    HMODULE dxgi;

    if (reason == DLL_PROCESS_ATTACH)
    {
        call.super.id = QEMU_SYSCALL_ID(CALL_INIT_DLL);
        call.reason = DLL_PROCESS_ATTACH;
        qemu_syscall(&call.super);
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        call.super.id = QEMU_SYSCALL_ID(CALL_INIT_DLL);
        call.reason = DLL_PROCESS_DETACH;
        qemu_syscall(&call.super);
    }

    return TRUE;
}

HRESULT WINAPI DllCanUnloadNow(void)
{
    WINE_FIXME("Stub\n");
    return S_FALSE;
}

HRESULT WINAPI DllRegisterServer(void)
{
    WINE_ERR("Should not be called on the wrapper.\n");
    return E_FAIL;
}

HRESULT WINAPI DllUnregisterServer(void)
{
    WINE_ERR("Should not be called on the wrapper.\n");
    return E_FAIL;
}

#else

static void qemu_init_dll(struct qemu_syscall *call)
{
    struct qemu_dll_init *c = (struct qemu_dll_init *)call;

    switch (c->reason)
    {
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
}

const struct qemu_ops *qemu_ops;

static const syscall_handler dll_functions[] =
{
    qemu_ComponentFactory_AddRef,
    qemu_ComponentFactory_create_host,
    qemu_ComponentFactory_CreateBitmap,
    qemu_ComponentFactory_CreateBitmapClipper,
    qemu_ComponentFactory_CreateBitmapFlipRotator,
    qemu_ComponentFactory_CreateBitmapFromHBITMAP,
    qemu_ComponentFactory_CreateBitmapFromHICON,
    qemu_ComponentFactory_CreateBitmapFromMemory,
    qemu_ComponentFactory_CreateBitmapFromSource,
    qemu_ComponentFactory_CreateBitmapFromSourceRect,
    qemu_ComponentFactory_CreateBitmapScaler,
    qemu_ComponentFactory_CreateColorContext,
    qemu_ComponentFactory_CreateColorTransformer,
    qemu_ComponentFactory_CreateComponentEnumerator,
    qemu_ComponentFactory_CreateComponentInfo,
    qemu_ComponentFactory_CreateDecoder,
    qemu_ComponentFactory_CreateDecoderFromFileHandle,
    qemu_ComponentFactory_CreateDecoderFromFilename,
    qemu_ComponentFactory_CreateDecoderFromStream,
    qemu_ComponentFactory_CreateEncoder,
    qemu_ComponentFactory_CreateEncoderPropertyBag,
    qemu_ComponentFactory_CreateFastMetadataEncoderFromDecoder,
    qemu_ComponentFactory_CreateFastMetadataEncoderFromFrameDecode,
    qemu_ComponentFactory_CreateFormatConverter,
    qemu_ComponentFactory_CreateMetadataReader,
    qemu_ComponentFactory_CreateMetadataReaderFromContainer,
    qemu_ComponentFactory_CreateMetadataWriter,
    qemu_ComponentFactory_CreateMetadataWriterFromReader,
    qemu_ComponentFactory_CreatePalette,
    qemu_ComponentFactory_CreateQueryReaderFromBlockReader,
    qemu_ComponentFactory_CreateQueryWriter,
    qemu_ComponentFactory_CreateQueryWriterFromBlockWriter,
    qemu_ComponentFactory_CreateQueryWriterFromReader,
    qemu_ComponentFactory_CreateStream,
    qemu_ComponentFactory_QueryInterface,
    qemu_ComponentFactory_Release,
    qemu_IMILBitmapImpl_AddRef,
    qemu_IMILBitmapImpl_CopyPalette,
    qemu_IMILBitmapImpl_CopyPixels,
    qemu_IMILBitmapImpl_GetPixelFormat,
    qemu_IMILBitmapImpl_GetResolution,
    qemu_IMILBitmapImpl_GetSize,
    qemu_IMILBitmapImpl_QueryInterface,
    qemu_IMILBitmapImpl_Release,
    qemu_IMILBitmapImpl_UnknownMethod1,
    qemu_IMILUnknown1Impl_AddRef,
    qemu_IMILUnknown1Impl_QueryInterface,
    qemu_IMILUnknown1Impl_Release,
    qemu_IMILUnknown2Impl_AddRef,
    qemu_IMILUnknown2Impl_QueryInterface,
    qemu_IMILUnknown2Impl_Release,
    qemu_IMILUnknown2Impl_UnknownMethod1,
    qemu_init_dll,
    qemu_WICBitmap_AddRef,
    qemu_WICBitmap_CopyPalette,
    qemu_WICBitmap_CopyPixels,
    qemu_WICBitmap_GetPixelFormat,
    qemu_WICBitmap_GetResolution,
    qemu_WICBitmap_GetSize,
    qemu_WICBitmap_Lock,
    qemu_WICBitmap_QueryInterface,
    qemu_WICBitmap_Release,
    qemu_WICBitmap_SetPalette,
    qemu_WICBitmap_SetResolution,
    qemu_WICBitmapDecoder_AddRef,
    qemu_WICBitmapDecoder_CopyPalette,
    qemu_WICBitmapDecoder_GetColorContexts,
    qemu_WICBitmapDecoder_GetContainerFormat,
    qemu_WICBitmapDecoder_GetDecoderInfo,
    qemu_WICBitmapDecoder_GetFrame,
    qemu_WICBitmapDecoder_GetFrameCount,
    qemu_WICBitmapDecoder_GetMetadataQueryReader,
    qemu_WICBitmapDecoder_GetPreview,
    qemu_WICBitmapDecoder_GetThumbnail,
    qemu_WICBitmapDecoder_Initialize,
    qemu_WICBitmapDecoder_QueryCapability,
    qemu_WICBitmapDecoder_QueryInterface,
    qemu_WICBitmapDecoder_Release,
    qemu_WICBitmapEncoder_AddRef,
    qemu_WICBitmapEncoder_Commit,
    qemu_WICBitmapEncoder_CreateNewFrame,
    qemu_WICBitmapEncoder_GetContainerFormat,
    qemu_WICBitmapEncoder_GetEncoderInfo,
    qemu_WICBitmapEncoder_GetMetadataQueryWriter,
    qemu_WICBitmapEncoder_Initialize,
    qemu_WICBitmapEncoder_QueryInterface,
    qemu_WICBitmapEncoder_Release,
    qemu_WICBitmapEncoder_SetColorContexts,
    qemu_WICBitmapEncoder_SetPalette,
    qemu_WICBitmapEncoder_SetPreview,
    qemu_WICBitmapEncoder_SetThumbnail,
    qemu_WICBitmapFrameDecode_AddRef,
    qemu_WICBitmapFrameDecode_CopyPalette,
    qemu_WICBitmapFrameDecode_CopyPixels,
    qemu_WICBitmapFrameDecode_GetColorContexts,
    qemu_WICBitmapFrameDecode_GetMetadataQueryReader,
    qemu_WICBitmapFrameDecode_GetPixelFormat,
    qemu_WICBitmapFrameDecode_GetResolution,
    qemu_WICBitmapFrameDecode_GetSize,
    qemu_WICBitmapFrameDecode_GetThumbnail,
    qemu_WICBitmapFrameDecode_QueryInterface,
    qemu_WICBitmapFrameDecode_Release,
    qemu_WICBitmapFrameEncode_AddRef,
    qemu_WICBitmapFrameEncode_Commit,
    qemu_WICBitmapFrameEncode_GetMetadataQueryWriter,
    qemu_WICBitmapFrameEncode_Initialize,
    qemu_WICBitmapFrameEncode_QueryInterface,
    qemu_WICBitmapFrameEncode_Release,
    qemu_WICBitmapFrameEncode_SetColorContexts,
    qemu_WICBitmapFrameEncode_SetPalette,
    qemu_WICBitmapFrameEncode_SetPixelFormat,
    qemu_WICBitmapFrameEncode_SetResolution,
    qemu_WICBitmapFrameEncode_SetSize,
    qemu_WICBitmapFrameEncode_SetThumbnail,
    qemu_WICBitmapFrameEncode_WritePixels,
    qemu_WICBitmapFrameEncode_WriteSource,
    qemu_WICBitmapLock_AddRef,
    qemu_WICBitmapLock_GetDataPointer,
    qemu_WICBitmapLock_GetPixelFormat,
    qemu_WICBitmapLock_GetSize,
    qemu_WICBitmapLock_GetStride,
    qemu_WICBitmapLock_QueryInterface,
    qemu_WICBitmapLock_Release,
    qemu_WICConvertBitmapSource,
    qemu_WICCreateBitmapFromSection,
    qemu_WICCreateBitmapFromSectionEx,
    qemu_WICMapGuidToShortName,
    qemu_WICMapSchemaToName,
    qemu_WICMapShortNameToGuid,
    qemu_WICPalette_AddRef,
    qemu_WICPalette_GetColorCount,
    qemu_WICPalette_GetColors,
    qemu_WICPalette_GetType,
    qemu_WICPalette_HasAlpha,
    qemu_WICPalette_InitializeCustom,
    qemu_WICPalette_InitializeFromBitmap,
    qemu_WICPalette_InitializeFromPalette,
    qemu_WICPalette_InitializePredefined,
    qemu_WICPalette_IsBlackWhite,
    qemu_WICPalette_IsGrayscale,
    qemu_WICPalette_QueryInterface,
    qemu_WICPalette_Release,
};

const WINAPI syscall_handler *qemu_dll_register(const struct qemu_ops *ops, uint32_t *dll_num)
{
    WINE_TRACE("Loading host-side windowscodecs wrapper.\n");

    qemu_ops = ops;
    *dll_num = QEMU_CURRENT_DLL;

    return dll_functions;
}

#endif