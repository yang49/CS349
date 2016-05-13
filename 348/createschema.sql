drop table if exists STUDENT;
drop table if exists COURSE;
drop table if exists PREREQUISITE;
drop table if exists PROFESSOR;
drop table if exists ENROLLMENT;
drop table if exists SCHEDULE;
drop table if exists CLASS;

create table STUDENT (SNO INTEGER NOT NULL, 
					SNAME TEXT collate nocase, 
					primary key (SNO)) ;

create table COURSE (CNO TEXT NOT NULL collate nocase, 
					CNAME TEXT collate nocase, 
					primary key (CNO))  ;

create table PREREQUISITE (CNO  TEXT NOT NULL collate nocase, 
						PREREQ  TEXT NOT NULL collate nocase, 
						primary key (CNO,PREREQ),
						foreign key (CNO) references COURSE(CNO),
						foreign key (PREREQ) references COURSE(CNO)) ;

create table PROFESSOR (EID INTEGER NOT NULL, 
					PNAME TEXT collate nocase, 
					OFFICE TEXT collate nocase, 
					DEPT TEXT collate nocase, 
					primary key (EID))  ;

create table CLASS (CNO  TEXT NOT NULL collate nocase, 
					TERM TEXT NOT NULL collate nocase, 
					SECTION INTEGER NOT NULL, 
					INSTRUCTOR INTEGER, 
					primary key (CNO, TERM, SECTION),
					foreign key (CNO) references COURSE(CNO),
					foreign key (INSTRUCTOR) references PROFESSOR(EID)) ;

create table ENROLLMENT (SNO INTEGER NOT NULL, 
					CNO TEXT NOT NULL collate nocase, 
					TERM TEXT NOT NULL collate nocase, 
					SECTION INTEGER NOT NULL, 
					MARK INTEGER, 
					primary key (SNO, CNO, TERM),
					foreign key (SNO) references STUDENT(SNO),
					foreign key (CNO, TERM, SECTION) references CLASS(CNO, TERM, SECTION)) ;

create table SCHEDULE (CNO  TEXT NOT NULL collate nocase, 
					TERM  TEXT NOT NULL collate nocase, 
					SECTION INTEGER NOT NULL, 
					DAY  TEXT NOT NULL collate nocase, 
					TIME TIME, 
					ROOM TEXT collate nocase, 
					primary key (CNO, TERM, SECTION, DAY),					
					foreign key (CNO, TERM, SECTION) references CLASS(CNO, TERM, SECTION))  ;



