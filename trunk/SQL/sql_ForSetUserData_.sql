------------------------------------
--   포인트에 맞는 Rank를 return  --
------------------------------------
create procedure GetRank
	@rank_point int
AS
	declare @tmpRank int;
	select @tmpRank=RANK_ID from RankTbl where RANK_NEED_POINT <= @rank_point;

	return @tmpRank;
GO

drop procedure GetRank;

declare @tmpRank int;
exec @tmpRank=GetRank @rank_point=6000;
print @tmpRank;


------------------------------------
--        User Data Update        --
------------------------------------
create procedure UpdateUserData
	@id int,
	@rank_point int,
	@kill int,
	@death int
AS
	
GO