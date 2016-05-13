-- Question: 6
-- The strategy behind the formulation of my answer:
--		First group the enrollment by cno, term and section. Calculate the how many courses a student
--    have taken and the course avg for a corresponding course, and find the professors who does not
--    meet the constrain. Use except to factor out them
--    find the pname through class table and professor table and the data with class size and avg mark
SELECT		p.pname,  t7.cno, t7.term, t7.section, t7.cap, t7.avg_mark
FROM			(SELECT		t2.instructor
				 FROM			(SELECT   c.instructor, t1.cno, t1.term, t1.section, t1.cap, t1.avg_mark
								 FROM		(SELECT		cno, term, section, COUNT(sno) AS cap, AVG(mark) AS avg_mark
											 FROM			enrollment
											 GROUP BY	cno, term, section) t1,
											 class c
								 WHERE    t1.cno = c.cno AND t1.term = c.term AND t1.section = c.section) t2
				 GROUP BY    t2.instructor, t2.cno, t2.term, t2.section

				 EXCEPT

				 SELECT		t4.instructor
				 FROM			(SELECT   c1.instructor, t3.cno, t3.term, t3.section, t3.cap, t3.avg_mark
								 FROM		(SELECT		cno, term, section, COUNT(sno) AS cap, AVG(mark) AS avg_mark
											 FROM			enrollment
											 GROUP BY	cno, term, section) t3,
											 class c1
								 WHERE    t3.cno = c1.cno AND t3.term = c1.term AND t3.section = c1.section) t4
				 GROUP BY    t4.instructor, t4.cno, t4.term, t4.section
				 HAVING		t4.cap < 10) t5,
				 
				 (SELECT   c2.instructor, t6.cno, t6.term, t6.section, t6.cap, t6.avg_mark
								 FROM		(SELECT		cno, term, section, COUNT(sno) AS cap, AVG(mark) AS avg_mark
											 FROM			enrollment
											 GROUP BY	cno, term, section) t6,
											 class c2
								 WHERE    t6.cno = c2.cno AND t6.term = c2.term AND t6.section = c2.section) t7,
				 professor p
WHERE 		 p.eid = t5.instructor AND t7.instructor = p.eid
				;
				
