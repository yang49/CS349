-- Question: 5
-- The strategy behind the formulation of my answer:
--    First find out all the students who meet the requirement
-- 	Then group the enrollment table by sno and calculate the average of the corresponding
--    student. And count how many courses he/she had taken.
--    In the end, find the student name with corresponding sno
SELECT  		 s.sname, t2.avg_mark
FROM			 (SELECT 			sno
							 FROM 			enrollment e
							 GROUP BY      sno
				  EXCEPT
				  SELECT 			sno
							 FROM 			enrollment e
							 WHERE 			mark < 80
							 GROUP BY      sno) tt1,
				 (SELECT   t1.sno, COUNT(*) AS num_C, AVG(t1.mark) AS avg_mark
				  FROM		(SELECT 			sno, cno, term, mark
				   	 		 FROM 			enrollment e
					   		 WHERE 			mark >= 80
						   	 GROUP BY      sno, cno, term
						   	 ORDER BY 		sno) t1
				             GROUP BY sno) t2,
				  student s
WHERE			 tt1.sno =s.sno AND tt1.sno = t2.sno AND t2.avg_mark
				  ;
