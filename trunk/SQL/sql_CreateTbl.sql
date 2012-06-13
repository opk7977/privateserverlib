Create Table RankTbl(
	RANK_ID int Primary Key not null,
	RANK_NAME nchar(10) not null,
	RANK_NEED_POINT int not null
);

Create Table AccountTbl(
	ID int Primary Key not null identity (1,1),
	USER_ID nvarchar(10) not null Unique,
	USER_PW nvarchar(15) not null,
	E_MAIL nvarchar(20) not null,
	RANK_ID int	not null default 1,
	RANK_POINT int not null default 0,
	ACCUMULATED_KILL int not null default 0,
	ACCUMULATED_DEATH int not null default 0,
);

-- drop table RankTbl;
-- drop table AccountTbl;