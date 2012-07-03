------------------------------------
--    �α��� Ȯ�� �������ν���    --
------------------------------------
create procedure TryLogin
	@U_id nchar(10),
	@U_pw nvarchar(15)
AS
	declare @tmpID int=0,@tmpUPW nvarchar(15);
	select @tmpUPW=USER_PW, @tmpID=ID from AccountTbl where USER_ID=@U_id COLLATE Korean_Wansung_CS_AS;
	
	-- id�� ������ -1 return
	IF ( @tmpID <= 0 )
		return -1;
	--END IF;
	
	--��й�ȣ�� Ʋ���� 0 return
	IF ( @tmpUPW COLLATE Korean_Wansung_CS_AS <> @U_pw COLLATE Korean_Wansung_CS_AS )
		return 0;
	ELSE
		return @tmpID;
GO

drop procedure TryLogin;

declare @result int;
exec @result=TryLogin @U_id="areah", @U_pw="aaa";
print @result;

------------------------------------
-- data�� return�޴� �������ν��� --
------------------------------------
create procedure GetUserData
	@id int
AS
	declare @tmpid int=0;
	select ID, USER_ID, RANK_ID, RANK_POINT, ACCUMULATED_KILL, ACCUMULATED_DEATH from AccountTbl where ID=@id;
GO

drop procedure GetUserData;

exec GetUserData @id=1;
