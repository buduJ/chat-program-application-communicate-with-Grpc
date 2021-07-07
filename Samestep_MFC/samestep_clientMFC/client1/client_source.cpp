#include "pch.h"
#include "samestep.pb.h"
#include <grpcpp/grpcpp.h>
#include "samestep.grpc.pb.h"
#include <thread>
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>  
#include <map>
#include <string> 


using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::ClientReaderWriter;
using grpc::Status;
using Simple::Server;
using Simple::Post;
using Simple::Id;

class ChatClient {
public:
	ChatClient(std::shared_ptr<Channel> channel, int id) :stub1(Server::NewStub(channel)), c_id(id) {}

	void sendMessage(const std::string &input,  int id) {
		// 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为
		ClientContext context;
		// 创建一个请求对象，用于打包要发送的请求数据
		// 创建一个响应对象，用于解包响要接收的应数据
		Post request, response;

		request.set_next_id(id);
		request.set_content(input);
		// 发送请求，接收响应
		Status st = stub1->ToS(&context, request, &response);


		//消息存储插入哈希表
		//myData.insert(std::pair<int, Post>(request.id(), request.content));

		//std::cout << "Server reply: " << myData[request.id()] << std::endl;

		//遍历输出+迭代器的使用
		//auto iter = myData.begin();//auto自动识别为迭代器类型unordered_map<int,string>::iterator
		//while (iter != myData.end())
		//{
		//	std::cout << iter->first << "," << iter->second << std::endl;
		//	++iter;
		//}


	}

	std::string recMessage() {
		// 创建一个客户端上下文。它可以用来向服务器传递附加的信息，以及可以调整某些RPC行为
		ClientContext context;
		// 创建一个请求对象，用于打包要发送的请求数据
		// 创建一个响应对象，用于解包响要接收的应数据
		Post response;
		Id id;
		id.set_id(c_id);
		// 发送请求，接收响应
		Status st = stub1->ToC(&context, id, &response);
		//输出信息
		std::string str = response.content();
		return str;
	}

	bool Message_Exist() {
		ClientContext context;
		Id id;
		id.set_id(c_id);
		Post message;
		Status st = stub1->Exist_Test(&context, id, &message);
		if (message.exist())
			return true;
		else
			return false;
	}

private:
	std::unique_ptr<Server::Stub> stub1;
	int c_id;
};

//线程1
//void thr_function1(ChatClient &client)
//{
//	while (true)
//	{
//		if (client.Message_Exist()) {
//			std::cout << client.recMessage() << std::endl;
//		}
//	}
//
//}
////线程2
//void thr_function2()
//{
//	ChatClient client2(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()),2);
//	char buffer[1024];
//    {
//		while (true)
//		{
//		std::cout << "Enter message: " << std::endl;
//		std::cin.getline(buffer, sizeof(buffer));
//		client2.sendMessage(buffer, 1);
//		
//			if (client2.Message_Exist()) 
//			{
//				std::cout << client2.recMessage() << std::endl;
//			}
//		}
//	
//	}
//}