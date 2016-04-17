Project Structure
Main
 - IR -> Maze -> Motor-Control <-> PID
   ^				    ^
   |			 	    |
   ----------------------------------

Maze
 - manhattan_dist()
 - update_distances()
 - fully_explored()
 - is_solved()
 - next_move()

Motor-Control
 - turn_left()
 - turn_right()
 - turn_around()
 - stop()

 - Motor
   - speed()
   - turn()
   - stop()

IR
 - has_front_wall()
 - has_left_wall()
 - has_right_wall()
 - front_wall_dist()
 - right_wall_dist()
 - left_wall_dist()
 - ir_to_dist()
