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
		// ����һ���ͻ��������ġ���������������������ݸ��ӵ���Ϣ���Լ����Ե���ĳЩRPC��Ϊ
		ClientContext context;
		// ����һ������������ڴ��Ҫ���͵���������
		// ����һ����Ӧ�������ڽ����Ҫ���յ�Ӧ����
		Post request, response;

		request.set_next_id(id);
		request.set_content(input);
		// �������󣬽�����Ӧ
		Status st = stub1->ToS(&context, request, &response);


		//��Ϣ�洢�����ϣ��
		//myData.insert(std::pair<int, Post>(request.id(), request.content));

		//std::cout << "Server reply: " << myData[request.id()] << std::endl;

		//�������+��������ʹ��
		//auto iter = myData.begin();//auto�Զ�ʶ��Ϊ����������unordered_map<int,string>::iterator
		//while (iter != myData.end())
		//{
		//	std::cout << iter->first << "," << iter->second << std::endl;
		//	++iter;
		//}


	}

	std::string recMessage() {
		// ����һ���ͻ��������ġ���������������������ݸ��ӵ���Ϣ���Լ����Ե���ĳЩRPC��Ϊ
		ClientContext context;
		// ����һ������������ڴ��Ҫ���͵���������
		// ����һ����Ӧ�������ڽ����Ҫ���յ�Ӧ����
		Post response;
		Id id;
		id.set_id(c_id);
		// �������󣬽�����Ӧ
		Status st = stub1->ToC(&context, id, &response);
		//�����Ϣ
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

//�߳�1
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
////�߳�2
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