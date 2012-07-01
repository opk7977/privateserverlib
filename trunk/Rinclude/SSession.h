#ifndef __SESSION__
#define __SESSION__

#include "SPacket.h"

class SSession : public SServerObj
{
private:
	//주고 받는 완료 포트의 구조체
	CUSTOMOVERLAPPED	m_OverlappedSend;
	CUSTOMOVERLAPPED	m_OverlappedRecv;

	//받을 버퍼
	char				m_chRecvBuf[DEFAULT_BUFFER_SIZE];
	//받은 크기
	int					m_iRecvSize;
	HANDLE				m_hRecvEvent;
	BOOL				m_bActive;

private:
	//접속자 정보/////////////////////////
	SOCKADDR			m_sockAddr;
	SOCKET				m_socket;
	//////////////////////////////////////

protected:
	//자신의 SessionID번호
	int					m_mySessionID;
	//자신의 iocp핸들값 = map의 키값
	//int					m_iIocpKey;
	//자신의 ip주소
	char				m_IPAddr[20];
	TCHAR				m_tIPAddr[20];

public:
	SSession(void);
	virtual ~SSession(void);

	//생성과 파괴
	virtual	void	OnCreate();
	virtual void	OnDestroy();

	inline void		SetSessionID( int sessionID ) { m_mySessionID = sessionID; }
	inline int		GetSessionID() { return m_mySessionID; }

	//====================================
	// 패킷을 받았을 때의 호출 함수
	//------------------------------------
	virtual void	Dispatch( DWORD bytesTransferred, OVERLAPPED* ov );
	void			DispatchReceive( DWORD bytesTransfer );
	//------------------------------------
	// Send 함수
	// 일반 send와 프로토콜 id만 보내는 함수
	//------------------------------------
	virtual	int		SendPacket( SPacket& packet, BOOL tudp = FALSE );
	int				SendPacket( unsigned short protocolID );
	//====================================
	// socket return
	SOCKET			GetSocket();
	// 세션의 접속이 끊어졌는지 확인
	BOOL			IsDisconnected();
	//소켓을 넘겨 받아 설정한다.
	void			SetSocket( SOCKET socket );
	//SOCKADDR을 설정한다
	void			SetSockAddr( SOCKADDR addr );
	//패킷이 오기를 기다리는 함수
	BOOL			WaitForPacketReceive();

	//====================================
	//받은 패킷을 해석하는 함수 정의
	//각 서버에서 재정의하여 사용하면 된다
	//------------------------------------
	virtual void	PacketParsing( SPacket& packet ) {}
	//====================================
};


#endif

