#ifndef __SESSION__
#define __SESSION__

#include "SPacket.h"

class SSession : public SServerObj
{
private:
	//�ְ� �޴� �Ϸ� ��Ʈ�� ����ü
	CUSTOMOVERLAPPED	m_OverlappedSend;
	CUSTOMOVERLAPPED	m_OverlappedRecv;

	//���� ����
	char				m_chRecvBuf[DEFAULT_BUFFER_SIZE];
	//���� ũ��
	int					m_iRecvSize;
	HANDLE				m_hRecvEvent;
	BOOL				m_bActive;

private:
	//������ ����/////////////////////////
	SOCKADDR			m_sockAddr;
	SOCKET				m_socket;
	//////////////////////////////////////

protected:
	//�ڽ��� SessionID��ȣ
	int					m_mySessionID;
	//�ڽ��� iocp�ڵ鰪 = map�� Ű��
	//int					m_iIocpKey;
	//�ڽ��� ip�ּ�
	char				m_IPAddr[20];
	TCHAR				m_tIPAddr[20];

public:
	SSession(void);
	virtual ~SSession(void);

	//������ �ı�
	virtual	void	OnCreate();
	virtual void	OnDestroy();

	inline void		SetSessionID( int sessionID ) { m_mySessionID = sessionID; }
	inline int		GetSessionID() { return m_mySessionID; }

	//====================================
	// ��Ŷ�� �޾��� ���� ȣ�� �Լ�
	//------------------------------------
	virtual void	Dispatch( DWORD bytesTransferred, OVERLAPPED* ov );
	void			DispatchReceive( DWORD bytesTransfer );
	//------------------------------------
	// Send �Լ�
	// �Ϲ� send�� �������� id�� ������ �Լ�
	//------------------------------------
	virtual	int		SendPacket( SPacket& packet, BOOL tudp = FALSE );
	int				SendPacket( unsigned short protocolID );
	//====================================
	// socket return
	SOCKET			GetSocket();
	// ������ ������ ���������� Ȯ��
	BOOL			IsDisconnected();
	//������ �Ѱ� �޾� �����Ѵ�.
	void			SetSocket( SOCKET socket );
	//SOCKADDR�� �����Ѵ�
	void			SetSockAddr( SOCKADDR addr );
	//��Ŷ�� ���⸦ ��ٸ��� �Լ�
	BOOL			WaitForPacketReceive();

	//====================================
	//���� ��Ŷ�� �ؼ��ϴ� �Լ� ����
	//�� �������� �������Ͽ� ����ϸ� �ȴ�
	//------------------------------------
	virtual void	PacketParsing( SPacket& packet ) {}
	//====================================
};


#endif

