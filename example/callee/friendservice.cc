#include <iostream>
#include <string>
#include <vector>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

class FriendService : public fixbug::FriendServiceRpc
{
public:
    std::vector<std::string> GetFriendsList(uint32_t userid)
    {
        std::cout << "do GetFriendsList service!" << userid << std::endl;
        std::vector<std::string> vec;
        vec.push_back("li yuchen");
        vec.push_back("tang runzhi");
        vec.push_back("qian binrong");
        vec.push_back("zhang yinjie");

        return vec;
    }

    // 重写基类方法
    void GetFriendsList(::google::protobuf::RpcController *controller,
                        const ::fixbug::GetFriendsListRequest *request,
                        ::fixbug::GetFriendsListResponse *response,
                        ::google::protobuf::Closure *done)
    {
        uint32_t userid = request->userid();
        std::vector<std::string> friendsList = GetFriendsList(userid);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for (std::string &name : friendsList)
        {
            std::string *p = response->add_friends();
            *p = name;
        }
        done->Run();
    }
};

int main(int argc, char **argv)
{
    LOG_INFO("first log message!");
    LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);

    // 调用框架的初始化操作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象。 把FriendService对象发布到rpc节点上
    RpcProvider provider;

    // FriendService是从FriendServiceRpc继承来 而后者又是从protobuf的Service基类继承来的
    provider.NotifyService(new FriendService());
    // 启动一个rpc服务发布节点 Run以后进程进入阻塞状态 等待远端的rpc请求
    provider.Run();
    return 0;
}