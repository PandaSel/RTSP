#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "RTSPServer.h"
#include "RTSPLiveStreamer.h"

using SERVER_CALLBACK = std::function<int(void *arg, CallbackParam *param)>;
using PATH_URL_MAP = std::unordered_map<std::string, std::string>;
class Server
{
public:
	Server(int port);
	~Server();
	void parseServerSession(const std::string& filePath);
	void addServerSessions(const std::string& url, const std::string& sessionName, int streamType = 0);
	void removeServerSessions();
private:
	// just for one instance
	std::vector<std::unique_ptr<RTSPLiveStreamer>> mVLiveStreamer;
	PATH_URL_MAP mPathUrlMap;
};