#include "Server.h"
#include <iostream>
#include <fstream>
#include <string>
#include "Stringhelper.h"
#include "assert.h"

Server::Server(int port)
{
	parseServerSession("RTSP_url.txt");

	const auto& rtspServer = RTSPServer::instance();
	rtspServer->startServer(port, [](void *arg, CallbackParam *param) -> int {

		Server* ptr = static_cast<Server*>(arg);
		switch (param->fType)
		{
		case OPEN_SERVER_SESSION:	
		{	
			auto dParam = dynamic_cast<OpenServerSessionParam *>(param);
			std::string sessionName = dParam->fRequestString;
			std::cout << "[callback] OPEN_SERVER_SESSION ,request string:" << dParam->fRequestString << std::endl;

			const auto& iter = ptr->mPathUrlMap.find(sessionName);
			if (iter != ptr->mPathUrlMap.end())
			{
				ptr->removeServerSessions();
				ptr->mVLiveStreamer.clear();
				ptr->addServerSessions(iter->second, sessionName, 0);
			}			
		}	
		break;
		case CLIENT_CONNECTED:
		{
			auto dParam = dynamic_cast<ClientConnectedParam *>(param);
			std::cout << "[callback] CLIENT_CONNECTED, sessionName:" << dParam->fSessionName 
				<< ", sock:" << dParam->fSock << ", ipAddr:" << dParam->fClientAddr.sin_addr.S_un.S_addr
				<< ", port:" << dParam->fClientAddr.sin_port << ", streamType:" << dParam->fStreamType 
				<< std::endl;
		}			
		break;
		case CLIENT_DISCONNECTED:
		{
			auto dParam = dynamic_cast<ClientDisconnectedParam *>(param);
			std::cout << "[callback] CLIENT_DISCONNECTED, sock:" << dParam->fSock << std::endl;			
		}
		break;
		default:
			break;
		}
		return 0;
	}, this);
}

Server::~Server()
{
	removeServerSessions();
	mVLiveStreamer.clear();
	const auto& rtspServer = RTSPServer::instance();
	rtspServer->stopServer();
	rtspServer->destroy();
}

void Server::parseServerSession(const std::string& fileName)
{
	std::ifstream rd_urls;
	static int channelId = 0;
	char* pCurrDir = new char[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, pCurrDir);	
	std::string currDir(pCurrDir);
	delete pCurrDir;

	std::string filePath = currDir + "\\" +  fileName;
	rd_urls.open(filePath);
	if (rd_urls.good())
	{
		std::string line;
		while (getline(rd_urls, line))
		{
			auto vec = regexsplit(line, ";");
			assert(vec.size() == 2);			
			mPathUrlMap[vec[0]] = vec[1];
		}
	}
	else
	{
		std::cout << "Error opening file " << filePath << std::endl;
		throw std::logic_error("Read file source error! fileName:" + filePath);
	}
	rd_urls.close();
}

void Server::addServerSessions(const std::string& url, const std::string& sessionName, int streamType)
{
	auto streamersPtr = std::make_unique<RTSPLiveStreamer>();
	streamersPtr->open(url.c_str(), streamType, sessionName.c_str());
	streamersPtr->run();
	mVLiveStreamer.emplace_back(std::move(streamersPtr));
}

void Server::removeServerSessions()
{
	for (const auto& streamPtr : mVLiveStreamer)
	{
		streamPtr->close();
	}
}