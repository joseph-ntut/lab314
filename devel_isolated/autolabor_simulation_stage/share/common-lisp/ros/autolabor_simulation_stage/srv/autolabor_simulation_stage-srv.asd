
(cl:in-package :asdf)

(defsystem "autolabor_simulation_stage-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
)
  :components ((:file "_package")
    (:file "Obstacle" :depends-on ("_package_Obstacle"))
    (:file "_package_Obstacle" :depends-on ("_package"))
  ))