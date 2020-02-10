package com.github.ainr.http

import java.io.FileOutputStream
import java.util.concurrent.SynchronousQueue

import akka.http.scaladsl.model.{HttpEntity, StatusCodes}
import akka.http.scaladsl.model.ContentTypes._
import akka.http.scaladsl.model.headers.`Content-Type`
import akka.http.scaladsl.server.ContentNegotiator.Alternative.MediaType
import akka.http.scaladsl.server.{PathMatcher, Route}

import scala.concurrent.duration._
import akka.http.scaladsl.server.Directives._
import akka.http.scaladsl.server.Directive0
import spray.json._

object PiServer {
  val server: Route = withoutSizeLimit {
    concat(
      path("png") {
        post {
          extractStrictEntity(3.second) {
            entity => {
              //println(entity.contentType)
              val data = entity.data
              println(data)
              //println(entity.data.toArray)
              val out = Some(new FileOutputStream("/tmp/Test.png"))
              out.get.write(entity.data.toArray)
              complete(StatusCodes.OK)
            }
          }
        }
      },
      path("json") {
        post {
          extractStrictEntity(3.second) {
            entity => {
              println(entity.contentType)
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