#ifndef __PROTOCOL__
#define __PROTOCOL__

enum PublicProtocol
{
	SC_FULL_SERVER = 0,
};

enum UpdateProtocol
{
	//server -> client _user�� ���������� �����Ͽ����� Ȯ���� �ִ� ��Ŷ
	//���� ����
	SC_CONNECTED_OK_UPDATESRV = 10,

	//clinet -> server _server���� user�� Ŭ�� ������ �Ѱ� ��	
	//int			-		client version
	CS_CLIENT_VER,
	
	//server -> client _user�� client�� update���ش�.
	//?????data????????
	SC_UPDATE_CLIENT,

	//client -> server _server���� ������Ʈ �ϷḦ �˸�
	//���� ����
	CS_UPDATE_COMPLATE,

	//server -> client _user���� ������ change����� ����
	//int			-		IpAddrCount
	//char			-		LoginSrvIpAddr
	//int			-		LoginSrvPortNum
	SC_CHANGESRV_TO_LOGIN,
	
};

enum LoginProtocol
{
	//server -> client _user�� ���������� �����Ͽ����� Ȯ���� �ִ� ��Ŷ
	//���� ����
	SC_CONNECTED_OK_LOGINSRV = 100,

	//client -> server _server���� loginȮ���� ��û�ϴ� ��Ŷ
	//int			-		id���ڿ� ����
	//char			-		id
	//int			-		password���ڿ� ����
	//char			-		password
	CS_USER_LOGIN,

	//server -> client _user���� �α��ν��� ��ȣ�� ����(���� ���� �ʴ� id)
	//�������
	SC_LOGIN_FAILED_ID,

	//server -> client _user���� �α��ν��� ��ȣ�� ����(pass Ʋ��)
	//�������
	SC_LOGIN_FAILED_PASS,

	//server -> client _user���� �α��μ��� ��ȣ�� ����
	//int			-		SesseionID
	SC_LOGIN_SUCCESS,

	//server -> client _user���� �κ�� ������ �ű� �� �ֵ��� ������
	//int			-		IpAddrCount
	//char			-		LobbySrvIpAddr
	//ungined short	-		LobbySrvPortNum
	SC_CHANGESRV_TO_LOBBY,
};

#endif