#include "samestep.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <memory>
#include <iostream>
#include <strstream>
#include <unordered_map>
#include <string>

using grpc::Status;
using grpc::Server;
using grpc_impl::ServerContext;
using grpc_impl::ServerWriter;
using grpc_impl::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerBuilder;
using Simple::Server;
using Simple::Post;
using Simple::Id;

//������ϣ�������洢��Ϣ��id
std::unordered_map<int, Post> myData;

// �̳������ɵ�Service�࣬ʵ�������麯��
class ServiceImpl :
	public Simple::Server::Service {
public:

	explicit ServiceImpl() {}

	grpc::Status ToS(ServerContext *context, const Post *input, Post *output) override {
		myData[input->next_id()] = *input;
		//output->set_content(input->content());
		Post message;
		message.set_exist(true);
		*output = message;
		return Status::OK;
	}

	grpc::Status ToC(ServerContext *context, const Id *input, Post *output) override {
		*output = myData[input->id()];
		myData.erase(input->id());
		return Status::OK;
	}

	Status Exist_Test(ServerContext *context, const Id *input, Post *output) override {
		auto it = myData.find(input->id());
		if (it != myData.end()) {
			Post message;
			message.set_exist(true);
			*output = message;
		}
		else {
			Post message;
			message.set_exist(false);
			*output = message;
		}
		return Status::OK;
	}
};

int main()
{
	// ���񹹽��������ڹ���ͬ�������첽����
	grpc::ServerBuilder builder;
	// �����������
	ServiceImpl service;
	// ��Ӽ����ĵ�ַ�Ͷ˿ڣ���һ����������������֤��ʽ������ѡ����֤
	builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

	// ע�����
	builder.RegisterService(&service);
	// ����������
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server Runing" << std::endl;
	// ���������ѭ����������ĳ������server->Shutdown()�Ż᷵�أ�
	server->Wait();

	return 0;
}