create procedure AllRank
AS
	select RANK_ID, RANK_NEED_POINT from RankTbl;
GO

drop procedure AllRank;

exec AllRank;