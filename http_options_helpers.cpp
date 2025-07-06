#pragma once
// HttpOptionsHelpers.h — tiny helpers to mutate CHttpRequest::COptions
// --------------------------------------------------
// Goal: Provide libcurl‑style free functions so user code can do e.g.:
//   CHttpRequest::COptions opt{};
//   http_add_method(opt, L"POST");
//   http_add_header(opt, L"Content-Type", L"application/x-www-form-urlencoded");
//   http_add_body(opt, "field1=value1");
//   auto resp = CHttpRequest::Request(L"https://example.com/test", opt, status);
// No change inside original HttpRequest.cpp is required.
// --------------------------------------------------

#include "HttpRequest.h"
#include <string>
#include <vector>

/* ===================== BASIC SETTERS ===================== */

inline void http_add_method(CHttpRequest::COptions& o, const std::wstring& m) {
    o.method = m;
}

inline void http_add_header(CHttpRequest::COptions& o,
                            const std::wstring& name,
                            const std::wstring& value) {
    o.headers[name] = value; // overwrite if exists (curl style)
}

inline void http_set_proxy(CHttpRequest::COptions& o, const std::wstring& proxy) {
    o.proxyList = proxy;
}

inline void http_set_user_agent(CHttpRequest::COptions& o, const std::wstring& ua) {
    o.userAgent = ua;
}

inline void http_add_body(CHttpRequest::COptions& o, const std::string& data) {
    o.data.assign(data.begin(), data.end());
}

inline void http_add_body(CHttpRequest::COptions& o, const std::vector<uint8_t>& bytes) {
    o.data = bytes; // copy
}

inline void http_set_insecure(CHttpRequest::COptions& o, bool v = true) {
    o.insecure = v;
}

/* ========== Convenience wrappers mirroring earlier names ========== */
#define HTTPADD_METHOD  http_add_method
#define HTTPADD_HEADER  http_add_header
#define HTTPSET_PROXY   http_set_proxy
#define HTTPSET_UA      http_set_user_agent
#define HTTPADD_BODY    http_add_body
#define HTTPSET_INSECURE http_set_insecure
