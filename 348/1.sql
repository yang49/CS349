-- Question: 1
-- The strategy behind the formulation of my answer:
-- 	First find the information of class when term = F10 and room = RCH122 from schedule table.
--    Then inner join with the course table on cno
--    Next inner join with the class table on cno, section and term to get the information about
--    eid in the professor table 


SELECT   myt.cname, myt.cno, prt.pname
FROM		(SELECT	tt.cno,tt.cname,tclass.instructor
			 FROM		(SELECT t2.cno, t2.cname, t1.term, t1.section
					 	 FROM	(SELECT cno, term, section FROM schedule WHERE term = 'F10' AND room = 'RCH122') t1 
					 	 INNER JOIN course t2
					 	 ON t1.cno = t2.cno) tt
		 	INNER JOIN class tclass
		 	ON tclass.cno = tt.cno AND tclass.term = tt.term AND tclass.section = tt.section) myt,
			professor prt
WHERE 	prt.eid = myt.instructor
ORDER BY myt.cno;
