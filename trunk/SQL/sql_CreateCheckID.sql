create procedure CheckID
	@check_uid nchar(10)
AS
	-- 변수 선언
	declare @tmpCount int = 0;
	select @tmpCount=COUNT(ID) from AccountTbl where USER_ID=@check_uid;
	-- 검색된 결과가 있다면 1을 return
	-- 없다면 0을 return
	return @tmpCount;
GO

drop procedure CheckID;

declare @result int=0;
exec @result=CheckID @check_uid="areah";

print @result;