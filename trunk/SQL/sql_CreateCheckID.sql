create procedure CheckID
	@check_uid nchar(10)
AS
	-- ���� ����
	declare @tmpCount int = 0;
	select @tmpCount=COUNT(ID) from AccountTbl where USER_ID=@check_uid;
	-- �˻��� ����� �ִٸ� 1�� return
	-- ���ٸ� 0�� return
	return @tmpCount;
GO

drop procedure CheckID;

declare @result int=0;
exec @result=CheckID @check_uid="areah";

print @result;