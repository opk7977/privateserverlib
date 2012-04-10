#pragma once

enum LOGIN_SERVER
{
	// server -> client
	// ������ ���������� ������ ���� ��ٴ� ��ȣ�� �����ش�.
	// �ڷ� ����
	SC_LOGIN_CONNECT_OK = 0,

	// client -> server
	// ������ �� ������ �α��� ��Ŷ�� ������
	// int			-		id ������ ũ��
	// TCHAR		-		id
	// int			-		pw ������ ũ��
	// TCHAR		-		pw
	CS_LOGIN_LOGIN,

	// server -> client
	// id�� �������� �ʴ´ٴ� ��Ŷ
	// �ڷ� ����
	SC_LOGIN_NONID,

	// server -> client
	// pw�� Ʋ�ȴٴ� ��Ŷ
	// �ڷ� ����
	SC_LOGIN_NOTPW,

	// server -> client
	// �α��� ���� �Լ�, game������ ip�� port��ȣ�� �ѱ��
	// int			-		�ڽ��� indexID
	// int			-		ipAddr�� ������ ũ��
	// char			-		ipAddr
	// int			-		port��ȣ
	SC_LOGIN_SUCCESS,
};

enum GAME_SERVER
{
	// server -> client
	// ���� ������ ���������� ���ӵǾ����� �˸�
	// �ڷ� ����
	SC_GAME_CONNECT_OK = 100,

	// client -> server
	// ������ ������ �Ǿ����� �ڽ��� ����ID�� ���� ĳ���������� �޾� �;� �Ѵ�
	// int			-		SessionID
	// int			-		strID�� ������ ũ��
	// TCHAR		-		strID
	CS_GAME_SEND_INDECX,

	// server -> client
	// �α��μ������� �Ѿ�� ������ ���� ���������� ������� ����
	// �ڷ� ����
	SC_GAME_CONNECT_FAILED,

	// server -> client
	// ���ӵ�, Ȥ�� ������ ĳ���Ϳ� ���� �������
	// int			-		�����ؾ� �ϴ� ĳ������ ��
	// int			-		character index Id
	// int			-		strID�� ������ ũ��
	// TCHAR		-		strID
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	SC_GAME_CREATE_CHARACTER,

	// client -> server
	// ���ӵ� ĳ���Ϳ� ���� ���� ���� ���
	// int			-		character index Id
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	CS_GAME_CHANGE_STATE,

	// server -> client
	// ���ӵ� ĳ���Ϳ� ���� ���� ���� ���
	// int			-		character index Id
	// int			-		charState
	// int			-		position X
	// int			-		position Y
	SC_GAME_CHANGE_STATE,

	// server -> client
	// ������ ���� �����Ǿ�� �ϴ� ĳ���Ϳ� ���� �Ƹ�
	// ���ӵ� ĳ���Ϳ� ���� ���� ���� ���
	// int			-		character index Id
	SC_GAME_DELETE_CHARACTER,
};