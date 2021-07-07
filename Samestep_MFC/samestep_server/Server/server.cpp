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

//创建哈希表用来存储消息与id
std::unordered_map<int, Post> myData;

// 继承自生成的Service类，实现三个虚函数
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
	// 服务构建器，用于构建同步或者异步服务
	grpc::ServerBuilder builder;
	// 创建服务对象
	ServiceImpl service;
	// 添加监听的地址和端口，后一个参数用于设置认证方式，这里选择不认证
	builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

	// 注册服务
	builder.RegisterService(&service);
	// 构建服务器
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "Server Runing" << std::endl;
	// 进入服务处理循环（必须在某处调用server->Shutdown()才会返回）
	server->Wait();

	return 0;
}