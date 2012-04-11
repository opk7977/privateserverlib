#pragma once

enum LOGIN_SERVER
{
	// server -> client
	// �α��� ������ ������ ���������� �Ǿ��ٴ� Ȯ�� ��Ŷ
	// ������ ����
	SC_LOGIN_CONNECT_OK = 10,

	// client -> server
	// ���������� ���� id�ߺ� Ȯ���� ���� ��Ŷ
	// int			-		id������ ũ��
	// TCHAR		-		id
	CS_LOGIN_CHECK_ID,

	// server -> client
	// id�ߺ� üũ�� ���� ������� return �� �ش�
	// -1(����) : �ߺ�/ 1(���) : ��밡��
	// int			-		�����
	SC_LOGIN_CHECK_ID_RESULT,

	// client -> server
	// ������ ����µ� �ʿ��� ������ �޴´�
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		password�������� ũ��
	// TCHAR		-		password
	// int			-		e-mail �������� ũ��
	// TCHAR		-		e-mail
	CS_LOGIN_CREATE_ACCOUNT,

	// server -> client
	// ���� ���� ����⿡ ���� ���
	// -1(����) : ����/ 1(���) : ����
	// int			-		�����
	SC_LOGIN_CREATE_RESULT,

	// client -> server
	// ������ �������� �α��� �õ�
	// int			-		id�������� ũ��
	// TCHAR		-		id
	// int			-		password�������� ũ��
	// TCHAR		-		password
	CS_LOGIN_LOGIN,

	// server -> client
	// �α��� ����� return�Ѵ�
	// ����� -1(����): id����/ 0: ���Ʋ��/ n(���): ����(sessionID)
	// --------����� �����̸� �߰��� data--------------------------
	// int			-		ip�ּ��� ������ ũ��
	// char			-		ip�ּ�
	// int			-		port��ȣ
	SC_LOGIN_LOGIN_RESULT,
};