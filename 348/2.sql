-- Question: 2
-- The strategy behind the formulation of my answer:
-- 	First find the information about those advanced courses which was taught
--    by a CS prof and held in S03.
--    Then find the those prerequisite course in S03 which was taught by a non-CS prof
--    In the end use inner join use information on the cno


SELECT DISTINCT big1.cno, big1.pname, big2.prereq,  big2.pname
FROM		(SELECT		pret.cno, pname, pret.prereq
			 FROM			(SELECT cno, term, instructor
							 FROM class
							 WHERE term = 'S03') t1,
							 prerequisite pret,
							 professor prot
			 WHERE       pret.cno = t1.cno AND (t1.instructor = prot.eid AND prot.dept = 'Computer Science')) big1,
			 
			(SELECT 	temp.cno, temp.prereq, prot2.pname
			 FROM		(SELECT 		preq.cno, preq.prereq
				  	 	 FROM			class c, prerequisite preq
					 	 WHERE      c.term = 'S03' AND c.cno = preq.cno) temp,
						 class c2,
						 professor prot2
			 WHERE    (temp.prereq = c2.cno AND c2.term = 'S03' AND c2.instructor = prot2.eid AND prot2.dept <> 'Computer Science')) big2
WHERE 	big1.cno = big2.cno
ORDER BY big1.cno, big1.pname, big2.prereq, big2.pname;
