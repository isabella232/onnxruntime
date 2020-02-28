// Copyright(C) 2020 Intel Corporation
// Licensed under the MIT License

#pragma once

#include <memory>
#include <inference_engine.hpp>
#include "core/session/onnxruntime_cxx_api.h"
#include "core/providers/openvino/ibackend.h"

namespace onnxruntime {
namespace openvino_ep {

bool IsDebugEnabled();

class VADMBackend : public IBackend {
 public:
  VADMBackend(const ONNX_NAMESPACE::ModelProto& model_proto, std::vector<int> input_indexes, std::vector<std::string> output_names,std::string device_id, InferenceEngine::Precision precision);

  void Infer(Ort::CustomOpApi& ort, OrtKernelContext* context) override;


 private:

  void StartAsyncInference(Ort::CustomOpApi& ort, std::vector<const OrtValue*> input_tensors, size_t batch_slice_idx, size_t infer_req_idx, std::vector<InferenceEngine::InferRequest::Ptr>& infer_requests, std::shared_ptr<InferenceEngine::CNNNetwork> ie_cnn_network);

  void CompleteAsyncInference(Ort::CustomOpApi& ort, std::vector<OrtValue*> output_tensors, size_t batch_slice_idx, size_t infer_req_idx, std::vector<InferenceEngine::InferRequest::Ptr>& infer_requests, std::shared_ptr<InferenceEngine::CNNNetwork> ie_cnn_network);

  std::vector<int> input_indexes_;
  std::vector<std::string> output_names_;
  mutable std::mutex compute_lock_;
  std::shared_ptr<InferenceEngine::CNNNetwork> ie_cnn_network_;
  std::vector<InferenceEngine::InferRequest::Ptr> infer_requests_;
  size_t num_inf_reqs_;
};
}  // namespace openvino_ep
}  // namespace onnxruntime
