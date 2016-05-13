-- Question: 3
-- The strategy behind the formulation of my answer:
-- 	First group the enrollment table by cno and term. At the same time calucate the mark
--    and the number of students who enrolled in corresponding course
--    Second, select the course with capacity >= 10 and find the instructor information
--    In the end, select the professor name with corresponding eid


SELECT 	p.pname, p.dept, t1.cno, t1.term, c.section, t1.avg_mark
FROM		(SELECT 			cno, term, COUNT(sno) AS cap, AVG(mark) AS avg_mark
			 FROM 			enrollment
			 GROUP BY 		cno, term) t1,
			class c,
			professor p 
WHERE		t1.cap >= 10 AND t1.avg_mark > 85 AND t1.cno = c.cno AND t1.term = c.term
			AND p.eid = c.instructor
ORDER BY p.pname, p.dept, c.cno
			;
