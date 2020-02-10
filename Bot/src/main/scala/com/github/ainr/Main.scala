package github.ainr.Main

import akka.actor.ActorSystem
import akka.event.Logging
import akka.http.scaladsl.Http
import akka.stream.ActorMaterializer
import com.github.ainr.http.PiServer

object Main extends App {

  implicit val system: ActorSystem = ActorSystem("simple-http")
  implicit val materializer: ActorMaterializer = ActorMaterializer()

  implicit val log = Logging(system, "main")

  val port = 8081

  val bindingFuture =
    Http().bindAndHandle(PiServer.server, "localhost", port)

  log.info(s"Server started at the port $port")
}