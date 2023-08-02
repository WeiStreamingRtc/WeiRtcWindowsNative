// Copyright 2020 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_NATIVE_IO_NATIVE_IO_MANAGER_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_NATIVE_IO_NATIVE_IO_MANAGER_H_

#include "third_party/blink/public/mojom/native_io/native_io.mojom-blink.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/core/execution_context/execution_context_lifecycle_observer.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_remote.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "third_party/blink/renderer/platform/wtf/vector.h"

namespace blink {

class ExecutionContext;
class ExceptionState;
class NativeIOFileSync;
class ScriptState;

class NativeIOManager final : public ScriptWrappable,
                              public ExecutionContextClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(NativeIOManager);

 public:
  explicit NativeIOManager(ExecutionContext*,
                           HeapMojoRemote<mojom::blink::NativeIOHost> backend);

  NativeIOManager(const NativeIOManager&) = delete;
  NativeIOManager& operator=(const NativeIOManager&) = delete;

  // Needed because of the
  // mojo::Remote<blink::mojom::NativeIOHost>
  ~NativeIOManager() override;

  ScriptPromise open(ScriptState*, String name, ExceptionState&);
  ScriptPromise Delete(ScriptState*, String name, ExceptionState&);
  ScriptPromise getAll(ScriptState*, ExceptionState&);

  NativeIOFileSync* openSync(String name, ExceptionState&);
  void deleteSync(String name, ExceptionState&);
  Vector<String> getAllSync(ExceptionState&);

  // GarbageCollected
  void Trace(Visitor* visitor) override;

 private:
  // Called when the mojo backend disconnects.
  void OnBackendDisconnect();

  // Task runner used by NativeIOFile mojo receivers generated by this API.
  const scoped_refptr<base::SequencedTaskRunner> receiver_task_runner_;

  // Wraps an always-on Mojo pipe for sending requests to the storage backend.
  HeapMojoRemote<mojom::blink::NativeIOHost> backend_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_NATIVE_IO_NATIVE_IO_MANAGER_H_
