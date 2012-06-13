--------------------------------------------
-- rank 계급을 추가
--------------------------------------------
Create procedure InsertRankItem
	@rank_id int,
	@rank_name nchar(10),
	@need_point int
AS
	insert into RankTbl values (@rank_id, @rank_name, @need_point);
GO

drop procedure InsertRankItem;

exec InsertRankItem @rank_id=1,		@rank_name="1",		@need_point=0;
exec InsertRankItem @rank_id=2,		@rank_name="2",		@need_point=100;	
exec InsertRankItem @rank_id=3,		@rank_name="3",		@need_point=500;	
exec InsertRankItem @rank_id=4,		@rank_name="4",		@need_point=1000;
exec InsertRankItem @rank_id=5,		@rank_name="5",		@need_point=2000;
exec InsertRankItem @rank_id=6,		@rank_name="6",		@need_point=3000;
exec InsertRankItem @rank_id=7,		@rank_name="7",		@need_point=4000;
exec InsertRankItem @rank_id=8,		@rank_name="8",		@need_point=5000;
exec InsertRankItem @rank_id=9,		@rank_name="9",		@need_point=6500;
exec InsertRankItem @rank_id=10,	@rank_name="10",	@need_point=8000;
exec InsertRankItem @rank_id=11,	@rank_name="11",	@need_point=10000;
exec InsertRankItem @rank_id=12,	@rank_name="12",	@need_point=15000;
exec InsertRankItem @rank_id=13,	@rank_name="13",	@need_point=20000;
exec InsertRankItem @rank_id=14,	@rank_name="14",	@need_point=25000;
exec InsertRankItem @rank_id=15,	@rank_name="15",	@need_point=30000;
exec InsertRankItem @rank_id=16,	@rank_name="16",	@need_point=40000;
exec InsertRankItem @rank_id=17,	@rank_name="17",	@need_point=50000;
exec InsertRankItem @rank_id=18,	@rank_name="18",	@need_point=70000;
exec InsertRankItem @rank_id=19,	@rank_name="19",	@need_point=100000;
exec InsertRankItem @rank_id=20,	@rank_name="20",	@need_point=150000;

--------------------------------------------
-- rank 의 계급명을 변경
--------------------------------------------
Create procedure UpdateRankName
	@rank_id int,
	@rank_name nchar(10)
AS
	update RankTbl set RANK_NAME=@rank_name where RANK_ID=@rank_id;
GO

drop procedure UpdateRankName;

exec UpdateRankName @rank_id=1, @rank_name="1";

--------------------------------------------
-- 계급의 need point를 변경
--------------------------------------------
Create procedure UpdateRankMinMaxPoint
	@rank_id int,
	@need_point int
AS
	update RankTbl set RANK_NEED_POINT=@need_point where RANK_ID=@rank_id;
GO

exec UpdateRankMinMaxPoint @rank_id=1, @need_point="1";

drop procedure UpdateRankMinMaxPoint;





