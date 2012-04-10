#ifndef __PROTOCOL__
#define __PROTOCOL__

enum PublicProtocol
{
	SC_FULL_SERVER = 0,
};

enum UpdateProtocol
{
	//server -> client _user가 정상적으로 접속하였음을 확인해 주는 패킷
	//내용 없음
	SC_CONNECTED_OK_UPDATESRV = 10,

	//clinet -> server _server에게 user의 클라 버젼을 넘겨 줌	
	//int			-		client version
	CS_CLIENT_VER,
	
	//server -> client _user의 client를 update해준다.
	//?????data????????
	SC_UPDATE_CLIENT,

	//client -> server _server에게 업데이트 완료를 알림
	//내용 없음
	CS_UPDATE_COMPLATE,

	//server -> client _user에게 서버의 change명령을 내림
	//int			-		IpAddrCount
	//char			-		LoginSrvIpAddr
	//int			-		LoginSrvPortNum
	SC_CHANGESRV_TO_LOGIN,
	
};

enum LoginProtocol
{
	//server -> client _user가 정상적으로 접속하였음을 확인해 주는 패킷
	//내용 없음
	SC_CONNECTED_OK_LOGINSRV = 100,

	//client -> server _server에게 login확인을 요청하는 패킷
	//int			-		id문자열 개수
	//char			-		id
	//int			-		password문자열 개수
	//char			-		password
	CS_USER_LOGIN,

	//server -> client _user에게 로그인실패 신호를 보냄(존재 하지 않는 id)
	//내용없음
	SC_LOGIN_FAILED_ID,

	//server -> client _user에게 로그인실패 신호를 보냄(pass 틀림)
	//내용없음
	SC_LOGIN_FAILED_PASS,

	//server -> client _user에게 로그인성공 신호를 보냄
	//int			-		SesseionID
	SC_LOGIN_SUCCESS,

	//server -> client _user에게 로비로 서버를 옮길 수 있도록 보내줌
	//int			-		IpAddrCount
	//char			-		LobbySrvIpAddr
	//ungined short	-		LobbySrvPortNum
	SC_CHANGESRV_TO_LOBBY,
};

#endif