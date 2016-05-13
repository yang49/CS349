-- Question: 4
-- The strategy behind the formulation of my answer:
--		First group the enrollment table by cno and term and calculate the corresponding
--    class capacity and class average.
--    Then select those students with grade on a course that is >= 98 and find that course
--    from the information I found in the first step.


SELECT		e.sno, e.cno, e.mark - avg_mark
FROM		 	(SELECT 			cno, term, COUNT(sno) AS cap, AVG(mark) AS avg_mark
			    FROM 			enrollment
			    GROUP BY 		cno, term) t1,
			   enrollment e
WHERE			(e.mark > 98 AND t1.cno = e.cno AND t1.term = e.term)
ORDER BY    e.sno, t1.cno
			 ;
