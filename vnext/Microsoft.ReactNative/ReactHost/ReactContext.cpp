// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include "ReactContext.h"
#include <winrt/Microsoft.ReactNative.h>
#include "Microsoft.ReactNative/IReactNotificationService.h"
#include "ReactInstanceWin.h"

namespace Mso::React {

//=============================================================================================
// ReactContext implementation
//=============================================================================================

ReactContext::ReactContext(
    Mso::WeakPtr<ReactInstanceWin> &&reactInstance,
    winrt::Microsoft::ReactNative::IReactPropertyBag const &properties,
    winrt::Microsoft::ReactNative::IReactNotificationService const &notifications) noexcept
    : m_reactInstance{std::move(reactInstance)}, m_properties{properties}, m_notifications{notifications} {}

void ReactContext::Destroy() noexcept {
  if (auto notificationService =
          winrt::get_self<winrt::Microsoft::ReactNative::implementation::ReactNotificationService>(m_notifications)) {
    notificationService->UnsubscribeAll();
  }
}

winrt::Microsoft::ReactNative::IReactPropertyBag ReactContext::Properties() noexcept {
  return m_properties;
}

winrt::Microsoft::ReactNative::IReactNotificationService ReactContext::Notifications() noexcept {
  return m_notifications;
}

void ReactContext::CallJSFunction(std::string &&module, std::string &&method, folly::dynamic &&params) noexcept {
#ifndef CORE_ABI // requires instance
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    instance->CallJsFunction(std::move(module), std::move(method), std::move(params));
  }
#endif
}

void ReactContext::DispatchEvent(int64_t viewTag, std::string &&eventName, folly::dynamic &&eventData) noexcept {
#ifndef CORE_ABI // requires instance
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    instance->DispatchEvent(viewTag, std::move(eventName), std::move(eventData));
  }
#endif
}

#ifndef CORE_ABI
ReactInstanceState ReactContext::State() const noexcept {
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    return instance->State();
  }

  return ReactInstanceState::Unloaded;
}

bool ReactContext::IsLoaded() const noexcept {
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    return instance->IsLoaded();
  }

  return false;
}

std::string ReactContext::GetBundleRootPath() const noexcept {
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    return instance->GetBundleRootPath();
  }

  return "";
}

facebook::react::INativeUIManager *ReactContext::NativeUIManager() const noexcept {
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    return instance->NativeUIManager();
  }

  return nullptr;
}

std::shared_ptr<facebook::react::Instance> ReactContext::GetInnerInstance() const noexcept {
  if (auto instance = m_reactInstance.GetStrongPtr()) {
    return instance->GetInnerInstance();
  }

  return nullptr;
}
#endif

} // namespace Mso::React
