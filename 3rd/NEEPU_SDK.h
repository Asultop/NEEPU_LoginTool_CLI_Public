#ifndef NEEPU_SDK_H
#define NEEPU_SDK_H
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cpr/cpr.h>
#include "json.hpp"
namespace NEEPULoginSDK{


using json = nlohmann::json;


struct LoginRequest {
	std::string user;     // 用户名
	std::string pass;     // 密码（已加密）
	std::string code;     // 验证码（可选）
	int authmode = 0;     // 认证模式
	std::string pool;     // 运营商池
	int isp_id = 0;       // ISP ID
	std::string pxyacct;  // 代理账号
};
json LoginRequest2Json(LoginRequest req){
	json req_json;
	req_json["user"] = req.user;
	req_json["pass"] = req.pass;
	req_json["code"] = req.code;
	req_json["authmode"] = req.authmode;
	req_json["pool"] = req.pool;
	req_json["isp_id"] = req.isp_id;
	req_json["pxyacct"] = req.pxyacct;
	return req_json;
}

struct LoginResponse {
	bool success;
	int retCode;
	std::string msg;
	json data;
};
std::string urlEncode(const std::string& str) {
    std::ostringstream encoded;
    encoded.fill('0');
    encoded << std::hex;

    for (char c : str) {
        // 不需要编码的字符：字母、数字和部分符号
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded << c;
            continue;
        }
        // 其他字符进行百分比编码
        encoded << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
    }

    return encoded.str();
}

std::string buildUrlParam(const json& params) {
    if (params.empty()) {
        return "";
    }

    std::vector<std::string> paramList;
    // 遍历JSON对象的所有键值对
    for (const auto& [key, value] : params.items()) {
        std::string valueStr;

        // 根据值的类型进行转换
        if (value.is_string()) {
            valueStr = value.get<std::string>();
        } else if (value.is_number()) {
            // 处理所有数字类型（int/double等）
            valueStr = std::to_string(value.get<int>());
        } else if (value.is_boolean()) {
            valueStr = value.get<bool>() ? "true" : "false";
        } else {
            // 忽略不支持的类型（如对象、数组等）
            continue;
        }

        // 编码值并添加到参数列表
        paramList.push_back(key + "=" + urlEncode(valueStr));
    }

    // 拼接所有参数，前面添加"?"
    std::string result;
    if (!paramList.empty()) {
        result += "?";
        for (size_t i = 0; i < paramList.size(); ++i) {
            if (i > 0) {
                result += "&";
            }
            result += paramList[i];
        }
    }

    return result;
}
LoginResponse sendLoginRequest(const std::string& api_url, const LoginRequest& req) {
	json req_json=LoginRequest2Json(req);
	
	// Build form-encoded body from buildUrlParam (removing leading '?')
	std::string body = buildUrlParam(req_json);
	if (!body.empty() && body[0] == '?') {
		body = body.substr(1);
	}
	
	// Make POST request using cpr
	auto res = cpr::Post(
		cpr::Url{api_url},
		cpr::Body{body},
		cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}}
	);
	
	LoginResponse resp;
	if (res.error.code != cpr::ErrorCode::OK) {
		resp.success = false;
		resp.retCode = -1;
		resp.msg = "请求失败：" + res.error.message;
		return resp;
	}
	
	if (res.status_code != 200) {
		resp.success = false;
		resp.retCode = res.status_code;
		resp.msg = "HTTP错误：" + std::to_string(res.status_code);
		return resp;
	}
	
	// 解析JSON响应
	try {
		json resp_json = json::parse(res.text);
		resp.retCode = resp_json["ret"].get<int>();
		resp.success = (resp.retCode == 0 || resp.retCode == 3);
		resp.msg = resp_json["msg"].get<std::string>();
		if (resp_json.contains("data")) {
			resp.data = resp_json["data"];
		}
	} catch (const std::exception& e) {
		resp.success = false;
		resp.retCode = -2;
		resp.msg = "JSON解析失败：" + std::string(e.what());
	}
	
	return resp;
}
LoginResponse sendGetStatusRequest(const std::string& api_url, const LoginRequest& req){
	json req_json=LoginRequest2Json(req);
	
	// Build form-encoded body from buildUrlParam (removing leading '?')
	std::string body = buildUrlParam(req_json);
	if (!body.empty() && body[0] == '?') {
		body = body.substr(1);
	}
	
	// Make POST request using cpr
	auto res = cpr::Post(
		cpr::Url{api_url},
		cpr::Body{body},
		cpr::Header{{"Content-Type", "application/x-www-form-urlencoded"}}
	);
	
	LoginResponse resp;
	if (res.error.code != cpr::ErrorCode::OK) {
		resp.success = false;
		resp.retCode = -1;
		resp.msg = "请求失败：" + res.error.message;
		return resp;
	}
	
	if (res.status_code != 200) {
		resp.success = false;
		resp.retCode = res.status_code;
		resp.msg = "HTTP错误：" + std::to_string(res.status_code);
		return resp;
	}
	
	// 解析JSON响应
	try {
		json resp_json = json::parse(res.text);
		resp.retCode = resp_json["ret"].get<int>();
		resp.success = (resp.retCode == 0 || resp.retCode == 3);
		resp.msg = resp_json["msg"].get<std::string>();
		if (resp_json.contains("data")) {
			resp.data = resp_json["data"];
		}
	} catch (const std::exception& e) {
		resp.success = false;
		resp.retCode = -2;
		resp.msg = "JSON解析失败：" + std::string(e.what());
	}
	
	return resp;
}

}
#endif