package com.github.ainr.http

import java.io.{BufferedOutputStream, File, FileOutputStream}
import java.nio.file.Paths
import java.util.concurrent.{Future, SynchronousQueue}

import akka.http.scaladsl.model.{HttpEntity, StatusCodes}
import akka.http.scaladsl.model.ContentTypes._
import akka.http.scaladsl.model.headers.`Content-Type`
import akka.http.scaladsl.server.ContentNegotiator.Alternative.MediaType
import akka.http.scaladsl.server.{PathMatcher, Route}

import scala.concurrent.duration._
import akka.http.scaladsl.server.Directives._
import akka.http.scaladsl.server.Directive0
import akka.http.scaladsl.server.directives.FileInfo
import akka.stream.scaladsl.FileIO
import spray.json._

import scala.util.{Failure, Success}

object PiServer {
  val server: Route = withoutSizeLimit {
    concat(
      path("png") {
        // curl -o /dev/null -X POST --header "Content-Type: image/png" --data-binary @img.png http://localhost:8081/png
        post {
          extractStrictEntity(3.second) {
            entity => {
              println(entity.getData())
              val bos = new BufferedOutputStream(new FileOutputStream("/tmp/i.png"))
              bos.write(entity.getData().toArray)
              bos.close()
              complete(StatusCodes.OK)
            }
          }
        }
      },
      path("json") {
        // curl --header "Content-Type: text/json" -d "{\"k\":\"1\", \"v\":2}" http://localhost:8081/png
        post {
          extractStrictEntity(3.second) {
            entity => {
              //println(entity.contentType)
              println(entity.data.utf8String.toString)
              val json = entity.data.utf8String.asJson.asJsObject.fields
              val v = json.getOrElse("v", "No such param")
              println(v)
              complete(StatusCodes.OK)
            }
          }
        }
      },
      path("html") {
        get {
          complete(StatusCodes.Created, List(`Content-Type`(`text/html(UTF-8)`)), "<html></html>")
        }
      }
    )
  }
}