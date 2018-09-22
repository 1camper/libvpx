;
;  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
;
;  Use of this source code is governed by a BSD-style license
;  that can be found in the LICENSE file in the root of the source
;  tree. An additional intellectual property rights grant can be found
;  in the file PATENTS.  All contributing project authors may
;  be found in the AUTHORS file in the root of the source tree.
;


%include "vpx_ports/x86_abi_support.asm"

section .text
global sym(vpx_clear_system_state) PRIVATE
sym(vpx_clear_system_state):
    emms
    ret

%macro CHECK_TAG 1
    fstenv   [rsp + %1]
    movsx    eax, word [rsp + %1 + 8]
    add      eax, 1
    sbb      eax, eax
%endmacro

global sym(vpx_check_system_state) PRIVATE
sym(vpx_check_system_state):
%if LIBVPX_YASM_WIN64
    CHECK_TAG 8    ; shadow space
%elif ARCH_X86_64
    CHECK_TAG -40  ; red zone
%else
    sub      esp, 28
    CHECK_TAG 0
    add      esp, 28
%endif
    ret
