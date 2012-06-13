create procedure InsertAccountItem
	@user_ID nchar(10),
	@user_PW nvarchar(15),
	@email nvarchar(20)
AS
	insert into AccountTbl (USER_ID, USER_PW, E_MAIL) values (@user_ID, @user_PW, @email);
GO

drop procedure InsertAccountItem;

exec InsertAccountItem @user_ID="areah", @user_PW="areah", @email="areah";
exec InsertAccountItem @user_ID="this", @user_PW="this", @email="this";
exec InsertAccountItem @user_ID="face", @user_PW="face", @email="face";
exec InsertAccountItem @user_ID="newsdu", @user_PW="newsdu", @email="newsdu";
exec InsertAccountItem @user_ID="Edward", @user_PW="Edward", @email="Edward";
