create procedure UpdateUserData
	@id int,
	@rank_Id int,
	@rank_point int,
	@kill int,
	@death int
AS
	update AccountTbl
	set RANK_ID=@rank_id, RANK_POINT=@rank_point, ACCUMULATED_KILL=@kill, ACCUMULATED_DEATH=@death
	where ID=@id;
GO

drop procedure UpdateUserData;


declare @rId int;
exec @rId=GetRank @rank_point=550;

exec UpdateUserData @id=1, @rank_id=@rId, @rank_point=550, @kill=55, @death=124;

select * from AccountTbl where ID=1;

exec @rId=GetRank @rank_point=0;
exec UpdateUserData @id=1, @rank_id=@rId, @rank_point=0, @kill=0, @death=0;
select * from AccountTbl where ID=1;
